#!/usr/bin/env python2

import argparse
import png
import operator
import random

parser = argparse.ArgumentParser(description='Compile font.')
parser.add_argument('source', help='input file')
parser.add_argument('name', help='name')
parser.add_argument('--ink', '-i', help='default foreground color', default=0, type=int)
parser.add_argument('--compress', '-c', action='store_true', help='compress bitmap with lz4 algorithm')
parser.add_argument('--algorithm', '-A', type=str, help='select algo', default='lz4')
parser.add_argument('--palette', '-p', nargs='+', help='palette')
parser.add_argument('--monochrome', '-m', action='store_true', help='do not generate attrs')
parser.add_argument('--format', '-f', type=str, help='output format', choices=['c', 'screen'], default='c')
parser.add_argument('--tile-size', '-t', type=int, help='attr tile size', default=4)
args = parser.parse_args()

tex = png.Reader(args.source)
w, h, pixels, metadata = tex.read_flat()
w = ((w + 7) >> 3) << 3

tw, th = args.tile_size, args.tile_size
nx = (w + tw - 1) // tw
ny = (h + th - 1) // th
data = bytearray([0] * ((tw * th * nx * ny) >> 3))
attrs = bytearray([0] * (nx * ny)) if not args.monochrome else bytearray()
palette = map(int, args.palette)

def get_pixel(x, y):
	if x < 0 or x >= w:
		return 0
	if y < 0 or y >= h:
		return 0

	return pixels[y * w + x]

def set_pixel(x, y):
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
			attr = (palette[bg] << 3 | args.ink)

		if args.monochrome:
			for yb in xrange(th):
				y = basey + yb
				for xb in xrange(tw):
					x = basex + xb
					v = get_pixel(x, y)
					if v != bg:
						set_pixel(x, y)
		else:
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
		compressed_attrs = bytearray(compress(attrs, mode='high_compression', compression=12)) if attrs else bytearray()
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

if args.format == 'c':
	hexdata = ", ".join(map(hex, data))
	source.append("static const u8 data[] = {%s};" %(hexdata))
	if attrs:
		hexdata = ", ".join(map(hex, attrs))
		source.append("static const u8 attrs[] = {%s};" %(hexdata))

	header.append('')
	header.append("#endif")

	header.append('')
	source.append('')
	source.append('const Texture texture_%s = { %d, %d, data, %s, %d, %d };' %(args.name, w, h, "attrs" if attrs else 0, palette[0], 1 if compressed else 0 ))
	source.append('')

	with open("texture_%s.h" %args.name, "wt") as f:
		f.write("\n".join(header))

	with open("texture_%s.c" %args.name, "wt") as f:
		f.write("\n".join(source))
elif args.format == 'screen':
	maxh = 192
	assert w == 256
	assert h <= maxh
	dy = (maxh - h) >> 4 << 3
	screen = bytearray([0] * (0x1800 + 0x300))
	source = []
	for y in xrange(dy, dy + h):
		srcy = (y - dy) << 5
		y0 = y & 7
		y1 = (y >> 3) & 7
		y2 = y >> 6
		dsty = (y2 << 11) | (y0 << 8) | (y1 << 5)
		screen[dsty: dsty + 32] = data[srcy: srcy + 32]

	for i in xrange(0x300):
		screen[0x1800 + i] = (palette[0] << 3) | palette[0]
	dy >>= 3
	for y in xrange(dy, dy + (h >> 3)):
		for x in xrange(32):
			a = attrs[(y - dy) * 32 + x]
			screen[0x1800 + y * 32 + x] = a

	source.append("; %d + %d bytes" %(len(data), len(attrs)))
	for b in screen:
		source.append(".db %d" %b)

	with open("screen_%s.s" %args.name, "wt") as f:
		f.write("""\
.module screen
.area CODE (ABS)
.org 0H4000

""" + "\n".join(source))
else:
	raise Exception('unsupported format %s' %args.format)
