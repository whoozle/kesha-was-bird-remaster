#!/usr/bin/env python2

from __future__ import print_function

import argparse
import wave
import struct
import sys
import os.path

parser = argparse.ArgumentParser(description='Convert audio.')
parser.add_argument('source', help='input file')
parser.add_argument('name', help='name')
parser.add_argument('destination', help='destination directory')
parser.add_argument('-e', '--encoding', help = 'encoder : [pdm|pwd]', default='pwm')
parser.add_argument('-c', '--cutoff', help = 'cutoff value', default=0.1, type=float)
parser.add_argument('-o', '--output', help = 'dump audio as wav file')
parser.add_argument('-l', '--level', type = int, default = 0, help = 'compression level')
args = parser.parse_args()

wav = wave.open(args.source)
n = wav.getnframes()
if wav.getsampwidth() != 2:
	raise Exception("invalid sample width")
if wav.getnchannels() != 1:
	raise Exception("only mono supported")
if wav.getframerate() != 4000:
	raise Exception("invalid sample rate, sox -S %s -r 4000 -b 16 <output>" %args.source)

frames = wav.readframes(n)

wavdata = bytes()
data = []
enc = args.encoding

qe = 0
peak = 0

offset = 0
for i in xrange(0, n):
	value, = struct.unpack('<h', frames[offset: offset + 2]) if offset < len(frames) else (0,)
	offset += 2
	if value > peak:
		peak = value
	elif value < -peak:
		peak = -value

offset = 0
size = 0
bit, byte = 0, 0
cutoff = args.cutoff
level = args.level

for i in xrange(0, n):
	buf = []

	value, = struct.unpack('<h', frames[offset: offset + 2]) if offset < len(frames) else (0,)
	offset += 2

	x = 1.0 * value / peak
	assert x >= -1 and x <= 1

	if enc == 'pdm':
		out = x >= qe
	elif enc == 'pwm':
		out = x >= cutoff
	else:
		raise Exception("unknown encoding " + enc)

	if out:
		byte |= (0x80 >> bit)
		y = 1
		wavdata += struct.pack('<h', 16384)
	else:
		y = -1
		wavdata += struct.pack('<h', -16384)

	if enc == 'pdm':
		qe = y - x + qe
		assert qe >= -1 and qe <= 1

	bit += 1
	if bit == 8:
		data.append(byte)
		bit = 0
		byte = 0
		size += 1

if size % 16:
	rem = 16 - (size % 16)
	for i in xrange(0, rem):
		size += 1

def compress(data):
	print("uncompressed data: %u bytes" %len(data), file=sys.stderr)
	pack, index = [], []

	def compare(pack, next, pos, n):
		bits = 0
		next_is_zero = sum(next) == 0
		level = args.level if not next_is_zero else 0 #zero tolerance
		for i in xrange(1, n):
			if pack[pos + i] != next[i]:
				bits += bin(pack[pos + i] ^ next[i]).count('1')
				if bits > level:
					return False
		return True

	def indexOf(next):
		first = next[0]
		n = len(next)
		try:
			pos = 0
			while True:
				pos = pack.index(first, pos, len(pack) - n)
				if compare(pack, next, pos, n):
					return pos
				else:
					pos += 1

		except ValueError:
			return -1

	for offset in xrange(0, len(data), 16):
		next = data[offset:offset + 16]
		src = indexOf(next)
		if src < 0:
			src = len(pack)
			pack += next
		index.append(src)

	total = len(pack) + len(index) * 2
	print("compressed data: %u + %u = %u bytes, level: %u, ratio: %.1f%%" %(len(pack), len(index) * 2, total, args.level, 100.0 * total / len(data)), file=sys.stderr)
	return pack, index

data, offsets = compress(data)

source, index = '', ''
source += "const u8 audio_%s_data[] = {\n" %args.name

for idx, byte in enumerate(data):
	mask = idx & 0x0f
	if mask == 0:
		source += '\t'
	source += '0x%02x' %byte
	if mask == 15:
		source += ',\n'
	else:
		source += ', '
source += "};\n"

for offset in offsets:
	index += "\t0x%04x," %offset
index += '\t0xffffu\n'

size /= 16 #loop count

with open(os.path.join(args.destination, "audio_%s.h" %args.name), "wt") as f:
	f.write("""\
#ifndef _AUDIO_{uname}_H
#define _AUDIO_{uname}_H

#include "types.h"

extern const u16 audio_{name}_index[];
extern const u8 audio_{name}_data[];

#endif
""".format(name = args.name, uname = args.name.upper()))

with open(os.path.join(args.destination, "audio_%s.c" %args.name), "wt") as f:
	f.write("""\
#include "audio_%s.h"
const u16 audio_%s_index[] = {
%s
};
%s"""  %(args.name, args.name, index, source))

if args.output:
	out = wave.open(args.output, 'w')
	out.setnchannels(1)
	out.setsampwidth(2)
	out.setframerate(wav.getframerate())
	out.setnframes(len(wavdata) / 2)
	out.writeframes(wavdata)
	out.close() #no __exit__
