#!/usr/bin/env python2

def generate(name, file, font_height = 5, space_width = 3):
	with open(file) as fontsource:
		font = {}
		while True:
			decl = fontsource.readline()
			if decl == '':
				break
			elif decl == '\n':
				continue
			decl = decl.split()
			if not decl:
				break
			if len(decl) == 3:
				char, height, width = (t(s) for t,s in zip((str, int, int), decl))
				descent = 0
			else:
				char, height, width, descent = (t(s) for t,s in zip((str, int, int, int), decl))

			if width > 8:
				raise "width is more than 8"

			rows = []
			for y in xrange(0, height):
				rowstr = fontsource.readline().rstrip()
				row = [1 if ch != ' ' else 0 for ch in rowstr]
				for x in xrange(len(row), width):
					row.append(0)

				#print y, row, shadow
				rows.append(row)

			font[char] = (height, width, descent, rows)

		#print font
		chars = [ord(x) - 31 for x in font.keys()]
		cmin, cmax = min(chars), max(chars)

		glyph = 0
		index_source = ""
		for ch in xrange(cmin, cmax + 1):
			key = chr(ch + 31)
			if key in font:
				height, width, descent, rows = font[key]
				glyph_data = []
				assert height == len(rows)
				for row in rows:
					value = 0
					for i in xrange(0, width):
						if row[i]:
							value |= (0x80 >> i)
					glyph_data.append(value)

				index_source += "\t{ %d, %d, %d, { %s } },\n" %(width, height, descent, ", ".join(["0x%02x" %x for x in glyph_data]))
				#print key, font[key]
				glyph += 1
			else:
				index_source += "\t{ %d, 0, 0 },\n" %space_width
	source = '#include "font_%s.h"\n\n' %name
	source += "const Glyph font_%s[%d] =\n{\n" %(name, cmax - cmin + 1)
	source += index_source
	source += "\n};\n"
	source += "\n\n"

	header = """\
#ifndef FONT_{uname}_H
#define FONT_{uname}_H
#include "font.h"

#define FONT_{uname}_SPACE_WIDTH ({space_width})
#define FONT_{uname}_CMIN ({min})
#define FONT_{uname}_CMAX ({max})
#define FONT_{uname}_GLYPHS ({glyphs})

extern const Glyph font_{name}[{glyphs}];

#endif
""".format(min = cmin, max = cmax, glyphs = (cmax - cmin + 1), name = name, uname=name.upper(), space_width = space_width)
	return header, source

import argparse
import os.path

parser = argparse.ArgumentParser(description='Compile font.')
parser.add_argument('source', help='input file')
parser.add_argument('name', help='input file')
parser.add_argument('target', help='target directory')
args = parser.parse_args()

decl, defn = generate(args.name, args.source)
with open(os.path.join(args.target, "font_%s.h" %args.name), "wt") as f:
	f.write(decl)

with open(os.path.join(args.target, "font_%s.c" %args.name), "wt") as f:
	f.write(defn)
