#include "fb.h"
#include <string.h>

u8 fbData[128 * 64 / 4];
u8 fbAttr[32 * 16];

static u8 dup4[16] =
{
	0x00, //0000
	0x03, //0001
	0x0c, //0010
	0x0f, //0011
	0x30, //0100
	0x33, //0101
	0x3c, //0110
	0x3f, //0111
	0xc0, //1000
	0xc3, //1001
	0xcc, //1010
	0xcf, //1011
	0xf0, //1100
	0xf3, //1101
	0xfc, //1110
	0xff, //1111
};

//clears X, increment line number
static u16 fb_next_line(u16 ptr)
{
	u8 y = ptr >> 5;
	if ((y & 0x38) == 0x38)
	{
		++y; //YY111111 case will increment y automatically or increment YYYYYY111
		y &= 0xc7; //wipe out lower 3 bits of y pos
	}
	else
		y += 8;
	return 0x4000 | (y << 5);
}

void fb_update(void) {
	u8 *dstBase = (u8 *)0x4080;
	const u8 *src = fbData;
	u8 lines = 64;
	while(lines--)
	{
		u8 *dst = dstBase;
		u8 width = 16;
		while(width--)
		{
			u8 srcByte = *src++;
			*dst++ = dup4[srcByte >> 4];
			*dst++ = dup4[srcByte & 0x0f];
		}
		u8 * next = (u8*)fb_next_line((u16)dstBase);
		memcpy(next, dstBase, 32);
		dstBase = (u8*)fb_next_line((u16)next);
	}
	memcpy(0x5880, fbAttr, 32 * 16);
}
