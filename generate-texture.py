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
parser.add_argument('--palette', '-p', nargs='+', help='palette')
args = parser.parse_args()

tex = png.Reader(args.source)
w, h, pixels, metadata = tex.read_flat()
tw, th = 4, 4
nx = (w + tw - 1) / tw
ny = (h + th - 1) / th
#data = bytearray([0] * (tw * th * nx * ny * 4 / 8))
data = bytearray([0] * 0x1800)
attrs = bytearray([0] * (nx * ny))
palette = map(int, args.palette)

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

def set_pixel2x(x, y):
	set_pixel(x * 2, y * 2)
	set_pixel(x * 2 + 1, y * 2)
	set_pixel(x * 2, y * 2 + 1)
	set_pixel(x * 2 + 1, y * 2 + 1)

if args.compress:
	packed_data = bytearray()
	if args.algorithm == "lz4":
		from lz4.block import compress
		compressed_data = bytearray(compress(packed_data, mode='high_compression', compression=12))
	elif args.algorithm == "huffman":
		compressed_data = huffman(packed_data)

	print "#compressed size: %d of %d\n" %(len(compressed_data), len(packed_data))
	print " ".join(map(lambda x: "0x%02x" %x, compressed_data))

else:
	for ty in xrange(ny):
		basey = ty * th
		for tx in xrange(nx):
			basex = tx * tw
			stats = {}
			for y in xrange(th):
				for x in xrange(tw):
					v = get_pixel(basex + x, basey + y)
					stats[v] = stats.setdefault(v, 0) + 1
			stats = stats.items()
			stats.sort(key=operator.itemgetter(1), reverse=True)
			stats = stats[:2]
			if len(stats) >= 2:
				bg, fg = stats[0][0], stats[1][0]
				attr = (palette[bg] << 3) | palette[fg]
			else:
				bg, fg = stats[0][0], 99
				attr = (palette[bg] << 3)

			attrs[ty * nx + tx] = attr

			for yb in xrange(th):
				y = basey + yb
				for xb in xrange(tw):
					x = basex + xb
					v = get_pixel(x, y)
					if fg == v:
						set_pixel2x(x, y)
					elif bg != v:
						set_pixel(x * 2, y * 2)
						#	set_pixel(x * 2 + 1, y * 2 + 1)
						#else:
						#	set_pixel(x * 2 + 1, y * 2)
						#	set_pixel(x * 2, y * 2 + 1)
						#if random.randint(0, 1) == 0:
						#	set_pixel(x * 2, y * 2)
						#if random.randint(0, 1) == 0:
						#	set_pixel(x * 2, y * 2 + 1)
						#if random.randint(0, 1) == 0:
						#	set_pixel(x * 2 + 1, y * 2)
						#if random.randint(0, 1) == 0:
						#	set_pixel(x * 2 + 1, y * 2 + 1)
						#if v & 1:
						#	set_pixel2x(x, y)
						#	set_pixel(x * 2, y * 2)
						#	set_pixel(x * 2 + 1, y * 2 + 1)

	hexdata = ", ".join(map(hex, data + attrs))
	#hexattrs = ", ".join(map(hex, attrs))
	print "const unsigned char %s_data[] = {%s};" %(args.name, hexdata)
	#print "unsigned char %s_attrs[] = {%s};" %(args.name, hexattrs)
