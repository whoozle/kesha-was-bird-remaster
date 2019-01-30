#include "runtime.h"

static u8 _last_value;
static __sfr __at 0xfe PortFE;

void border(u8 colour)
{
	_last_value = (_last_value & (u8)0xf8) | (colour & 7);
	PortFE = _last_value;
}

void speaker(u8 bit)
{
	_last_value = (_last_value & (u8)0xef) | (bit? (u8)0x10: (u8)0x00);
	PortFE = _last_value;
}

static u16 _rand = 0xACE1u;

void srand_r()
{
	_rand += get_r();
}

u16 rand()
{
    _rand ^= _rand << 7;
    _rand ^= _rand >> 9;
    _rand ^= _rand << 8;
	return _rand;
}
