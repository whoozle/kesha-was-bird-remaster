#!/usr/bin/env python3

import argparse
import struct

TYPE_DATA		= 0x00
TYPE_EOF		= 0x01
TYPE_EXT_SEG	= 0x02
TYPE_START_SEG	= 0x03
TYPE_EXT_LA		= 0x04
TYPE_START_LA	= 0x05

RAM_START	= 0x4000
RAM_SIZE	= 0xc000

class Image(object):
	def __init__(self):
		self.ram = bytearray([0] * RAM_SIZE)

	def load(self, data):
		data = str(data).split()
		for rec in data:
			if not rec or rec[0] != ':':
				print('invalid start code')
				continue

			rec = bytes.fromhex(rec[1:])
			if sum(rec) & 0xff:
				print('invalid checksum')
				continue

			size, addr, type = struct.unpack('>BHB', rec[0:4])
			data, checksum = rec[4:-1], rec[-1]
			#print(type, addr, size, data, checksum)
			if type == TYPE_EOF:
				break

			if addr < RAM_START:
				print('skipping record', hex(addr), size)
				continue

			begin, end = addr - RAM_START, addr + size - RAM_START
			self.ram[begin:end] = data

	def save(self, pc):
		header = struct.pack('<HHHHHBBBHHHHHHHBBB',
		0, #AF
		0, 0, #bc, hl
		pc,
		0xfff8, #sp
		0, #I
		0, #refresh
		0 << 1, #border + R7
		0, 0, 0, 0, #DE, + shadow BC, DE, HL
		0, 0, 0, #shadow AF, IX, IY
		0, #interrupt
		0, #IFF2
		0, #interrupt mode + flags
		)
		return header + self.ram

parser = argparse.ArgumentParser(description='Converts from ihx to sna')
parser.add_argument('source', help='input file')
parser.add_argument('destination', help='output file (sna)')
parser.add_argument('pc', help='pc', type=int)
args = parser.parse_args()

image = Image()
with open(args.source) as f:
	image.load(f.read())

with open(args.destination, 'wb') as f:
	f.write(image.save(args.pc))
