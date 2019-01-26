#!/usr/bin/env python2

import argparse
import os.path
import json

parser = argparse.ArgumentParser(description='Compile font.')
parser.add_argument('target', help='target directory')
parser.add_argument('sources', help='input file', nargs='+')
args = parser.parse_args()

data = []
lookup = {}

header = '#ifndef KESHA_TEXT_H\n#define KESHA_TEXT_H\n\n#include "types.h"\n\n'

def add(key, value):
	global header, data, lookup

	if value in lookup:
		index = lookup[value]
		header += "#define text_%s (%d)\n" %(key, index)
		return

	index = len(data)
	lookup[value] = index
	header += "#define text_%s (%d)\n" %(key, index)

	bytes = bytearray()
	for ch in value:
		if ch == '\n':
			v = 0xff
		else:
			v = ord(ch) - 31

		if v < 0:
			raise Exception('invalid character %s' %repr(ch))
		bytes.append(v)
	bytes.append(0)
	data.append(bytes)

for source in args.sources:
	messages = json.load(open(source))
	for key, value in messages.iteritems():
		if isinstance(value, list):
			for idx, text in enumerate(value, 1):
				add('%s_%d' %(key, idx), text)
		else:
			add(key, value)

header += "\nextern const u8* text_index[%d];\n\n" %len(data)

header += '\n#endif\n'

source = '#include "text.h"\n#include "types.h"\n\n'
source_index = ''
for idx, bytes in enumerate(data):
	source += 'static const u8 text_data_%d[] = { ' %idx + ", ".join(["0x%02x" %i for i in bytes]) + ' };\n'
source_index += "const u8 * text_index[%d] = {\n" %len(data)
for idx, bytes in enumerate(data):
	source_index += "\ttext_data_%d,\n" %idx
source_index += "};\n"

with open(os.path.join(args.target, 'text.h'), 'w') as f:
	f.write(header)

with open(os.path.join(args.target, 'text.c'), 'w') as f:
	f.write(source + source_index)
