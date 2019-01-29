#!/usr/bin/env python2

import argparse
import png
import operator
import random

parser = argparse.ArgumentParser(description='Compile font.')
parser.add_argument('source', help='input file')
parser.add_argument('name', help='name')
parser.add_argument('--background', '-b', help='background-color', default=0, type=int)
parser.add_argument('--compress', '-c', action='store_true', help='compress bitmap with lz4 algorithm')
parser.add_argument('--algorithm', '-A', type=str, help='select algo', default='lz4')
parser.add_argument('--palette', '-p', nargs='+', help='palette')
parser.add_argument('--monochrome', '-m', action='store_true', help='do not generate attrs')
args = parser.parse_args()

tex = png.Reader(args.source)
w, h, pixels, metadata = tex.read_flat()
w = ((w + 7) >> 3) << 3

tw, th = 4, 4
nx = (w + tw - 1) // tw
ny = (h + th - 1) // th
data = bytearray([0] * ((tw * th * nx * ny) >> 3))
attrs = bytearray([0] * (nx * ny))
palette = map(int, args.palette)

def get_pixel(x, y):
	if x < 0 or x >= w:
		return 0
	if y < 0 or y >= h:
		return 0

	return pixels[y * w + x]

def set_pixel(x, y):
	assert y < 64
	bit = x & 7
	addr = (y * (w >> 3)) + (x >> 3)
	data[addr] |= 0x80 >> bit

for ty in xrange(ny):
	basey = ty * th
	for tx in xrange(nx):
		basex = tx * tw
		stats = {}
		for yb in xrange(th):
			y = basey + yb
			for xb in xrange(tw):
				x = basex + xb
				v = get_pixel(x, y)
				stats[v] = stats.setdefault(v, 0) + 1
		stats = stats.items()
		stats.sort(key=operator.itemgetter(1), reverse=True)
		if len(stats) >= 3:
			bg, fg, fg2 = stats[0][0], stats[1][0], stats[2][0]
			attr = (palette[bg] << 3) | palette[fg]
		elif len(stats) == 2:
			bg, fg, fg2 = stats[0][0], stats[1][0], 99
			attr = (palette[bg] << 3) | palette[fg]
		else:
			bg, fg, fg2 = stats[0][0], 99, 99
			attr = (palette[bg] << 3)

		attrs[ty * nx + tx] = attr

		for yb in xrange(th):
			y = basey + yb
			for xb in xrange(tw):
				x = basex + xb
				v = get_pixel(x, y)
				if fg == v or fg2 == v:
					set_pixel(x, y)
				elif bg != v:
					set_pixel(x, y)

header, source = [], []
header.append("#ifndef TEXTURE_%s_H" %args.name.upper())
header.append("#define TEXTURE_%s_H" %args.name.upper())
header.append('')
header.append('#include "texture.h"')
header.append('')
header.append("extern const Texture texture_%s;" %(args.name))
header.append('')
header.append("#define TEXTURE_%s_DATA_SIZE (%d)" %(args.name.upper(), len(data)))
header.append("#define TEXTURE_%s_ATTRS_SIZE (%d)" %(args.name.upper(), len(attrs)))
header.append('')

source.append('#include "texture_%s.h"' %args.name)
source.append('')

def compress(data, attrs, compression = None):
	if compression is None:
		return data, attrs, False
	elif compression == 'lz4':
		from lz4.block import compress
		compressed_data = bytearray(compress(data, mode='high_compression', compression=12))
		compressed_attrs = bytearray(compress(attrs, mode='high_compression', compression=12))
	else:
		raise Exception("unknown compression %s" %args.algorithm)

	ctotal, total = len(compressed_data) + len(compressed_attrs), len(data) + len(attrs)
	if ctotal >= total:
		return data, attrs, False

	header.append("//compressed size: %d+%d (%d) of %d+%d (%d) = %d%%\n" %(len(compressed_data), len(compressed_attrs), ctotal, len(data), len(attrs), total, 100 * ctotal // total))
	header.append("#define TEXTURE_%s_DATA_CSIZE (%d)" %(args.name.upper(), len(compressed_data)))
	header.append("#define TEXTURE_%s_ATTRS_CSIZE (%d)" %(args.name.upper(), len(compressed_attrs)))
	return compressed_data, compressed_attrs, True


compression = None
if args.compress:
	compression = args.algorithm

data, attrs, compressed = compress(data, attrs, compression)
hexdata = ", ".join(map(hex, data))
source.append("static const u8 data[] = {%s};" %(hexdata))
hexdata = ", ".join(map(hex, attrs))
source.append("static const u8 attrs[] = {%s};" %(hexdata))

header.append('')
header.append("#endif")

header.append('')
source.append('')
source.append('const Texture texture_%s = { %d, %d, data, attrs, %d, %d };' %(args.name, w, h, palette[0], 1 if compressed else 0 ))
source.append('')

with open("texture_%s.h" %args.name, "wt") as f:
	f.write("\n".join(header))

with open("texture_%s.c" %args.name, "wt") as f:
	f.write("\n".join(source))
