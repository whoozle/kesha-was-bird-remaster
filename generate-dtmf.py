#!/usr/bin/env python2

FREQ = 4000
from math import sin, pi
from argparse import ArgumentParser

parser = ArgumentParser(description="Generate DTFM waveforms.")
parser.add_argument('-c', '--cutoff', help = 'cutoff value', default=0.0, type=float)
args = parser.parse_args()

def osc(t, freq):
	return sin(t * 2 * pi * freq)

def pulse(freq):
	buf = []
	for i in xrange(0, 128):
		v = osc(i * 1.0 / FREQ, freq)
		buf.append(v)
	return buf

def mix(a, b):
	c = []
	for i in xrange(0, 128):
		c.append(a[i] + b[i])
	return c

cols = (pulse(1209), pulse(1336), pulse(1477), pulse(1633))
rows = (pulse(697), pulse(770), pulse(852), pulse(941))
rosit = (pulse(913), pulse(1428), pulse(1776))
dial_tone = mix(pulse(350), pulse(440))

mapping = (
	1, 2, 3, 12,
	4, 5, 6, 13,
	7, 8, 9, 14,
	10, 0, 11, 15
)

def generate(tone):
	data = []
	byte = 0
	bit = 0
	activation = args.cutoff
	for i in xrange(0, 128):
		x = tone[i]
		if x >= activation:
			byte |= (0x80 >> bit)

		bit += 1
		if bit == 8:
			data.append("0x%02x" %byte)
			bit = 0
			byte = 0
	return ", ".join(data)

print("#include \"types.h\"\n\nconst u8 audio_phone_tones[20][16] = {\n{"),
tones = []
for row in xrange(0, 4):
	for col in xrange(0, 4):
		idx = row * 4 + col
		#print row, col, mapping[idx]
		tone = mix(rows[row], cols[col])
		#print tone
		tones.append(generate(tone))

for tone in rosit: #16, 17, 18
	tones.append(generate(tone))

tones.append(generate(dial_tone)) #19
print "}, {".join(tones)
print "}};"
