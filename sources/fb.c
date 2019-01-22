#include "fb.h"
#include <string.h>

u8 fb[128 * 64 / 4];

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
	const u8 *src = fb;
	u8 lines = 64;
	while(lines--)
	{
		u8 *dst = dstBase;
		u8 width = 32;
		while(width--)
		{
			u8 srcByte = *src++;
			// for(u8 b = 4; b--; )
			// {
			// 	u8 v = (srcByte >> (b << 1)) & 3;

			// }
			*dst++ = srcByte;
		}
		u8 * next = (u8*)fb_next_line((u16)dstBase);
		memcpy(next, dstBase, 32);
		dstBase = (u8*)fb_next_line((u16)next);

		// u8 byteWidth = 32;
		// while(byteWidth--)
		// {

		// }
	}
}
