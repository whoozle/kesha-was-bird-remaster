#!/usr/bin/env python2

import argparse
import png
import operator
import random

parser = argparse.ArgumentParser(description='Compile font.')
parser.add_argument('source', help='input file')
parser.add_argument('name', help='name')
parser.add_argument('--compress', '-c', action='store_true', help='compress bitmap with lz4 algorithm')
parser.add_argument('--algorithm', '-A', type=str, help='select algo', default='lz4')
args = parser.parse_args()

tex = png.Reader(args.source)
w, h, pixels, metadata = tex.read_flat()
data = bytearray([0] * (w * h))

def get_pixel(x, y):
	if x < 0 or x >= w:
		return 0
	if y < 0 or y >= h:
		return 0

	return pixels[y * w + x]

def set_pixel(x, y):
	assert y < 128
	bit = x & 7
	x >>= 3
	y2, y1, y0 = y >> 6, (y >> 3) & 7, y & 7
	addr = (((y2 << 6) | (y0 << 3) | y1) << 5) | x
	data[addr] |= 0x80 >> bit

for y in xrange(h):
	for x in xrange(w):
		v = get_pixel(x, y)
		data[y * w + x] = v

header, source = [], []
header.append("#ifndef TEXTURE_%s_H" %args.name.upper())
header.append("#define TEXTURE_%s_H" %args.name.upper())
header.append('')
header.append("extern const unsigned char %s_data[];" %(args.name))
header.append("#define %s_DATA_SIZE (%d)" %(args.name.upper(), len(data)))

if args.compress:
	packed_data = bytearray()
	if args.algorithm == "lz4":
		from lz4.block import compress
		compressed_data = bytearray(compress(data, mode='high_compression', compression=12))
	else:
		raise Exception("unknown compression %s" %args.algorithm)

	header.append("//compressed size: %d of %d\n" %(len(compressed_data), len(packed_data)))
	header.append("#define %s_DATA_CSIZE (%d)" %(args.name.upper(), len(compressed_data)))
	hexdata = ", ".join(map(hex, compressed_data))
	source.append("const unsigned char %s_data[] = {%s};" %(args.name, hexdata))
else:
	hexdata = ", ".join(map(hex, data))
	source.append("const unsigned char %s_data[] = {%s};" %(args.name, hexdata))

header.append('')
header.append("#endif")

header.append('')
source.append('')

with open("texture_%s.h" %args.name, "wt") as f:
	f.write("\n".join(header))

with open("texture_%s.c" %args.name, "wt") as f:
	f.write("\n".join(source))
