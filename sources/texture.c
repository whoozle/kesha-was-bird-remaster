#include "texture.h"
#include "fb.h"
#include "runtime.h"
#include "lz4.h"
#include <string.h>

void texture_draw_fullscreen(const Texture *texture)
{
	border(texture->background);
	fb_clear_attrs(texture->background << 3);
	lz4_unpack(fbData, texture->data);
	lz4_unpack(fbAttr, texture->attrs);
}

void texture_draw(const Texture *texture, u8 x, u8 y)
{
	u8 w = texture->width, h = texture->height;
	u8 wb = (w >> 3);
	u8 pitch = 16 - wb;

	u16 dstOffset = ((u16)y << 4) + (x >> 3);
	u8 * dst = fbData + dstOffset;
	const u8 * src = texture->data;
	u8 bit = x & 7;
	if (bit == 0)
	{
		while(h--)
		{
			u8 wcopy = wb;
			while(wcopy--)
				*dst++ ^= *src++;
			dst += pitch;
		}
	}
	else
	{
		while(h--)
		{
			u8 wcopy = wb;
			while(wcopy--)
			{
				u8 srcByte = *src++;
				u8 b1 = srcByte >> bit;
				u8 b2 = srcByte << (8 - bit);
				*dst++ ^= b1;
				*dst ^= b2;
			}
			dst += pitch;
		}
	}

}
