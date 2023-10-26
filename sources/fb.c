#include "fb.h"
#include "runtime.h"
#include <string.h>

u8 fbData[128 * 64 / 8];
u8 fbAttr[32 * 16];

static const u8 dup4[16] =
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
	u8 x = (u8)ptr & (u8)0x1f;
	u8 y = ptr >> 5;
	if ((y & (u8)0x38) == (u8)0x38)
	{
		++y; //YY111111 case will increment y automatically or increment YYYYYY111
		y &= (u8)0xc7; //wipe out lower 3 bits of y pos
	}
	else
		y += (u8)8;
	return (u16)0x4000u | ((u16)y << 5) | x;
}

u8 * fb_get_base_addr(u8 x, u8 y)
{
	//0 1 0 [Y7 Y6] [Y2 Y1 Y0]    [Y5 Y4 Y3] [X4 X3 X2 X1 X0]
	u8 y0 = y & 7;
	u8 y1 = (y << 2) & (u8)0xe0;
	u8 y2 = (y >> 3) & (u8)0x18;
	return (u8 *) (((u16)(0x40 | y2 | y0) << 8) | y1 | (x >> 3));
}

void fb_update_rect_attrs(u8 ax, u8 ay, u8 aw, u8 ah)
{
	u16 offset = FB_ATTR_OFFSET(ax, ay);
	u8 * dst = VRAM_ATTRS + offset;
	u8 * src = fbAttr + offset;
	u8 pitch = 32 - aw;
	while(ah--)
	{
		memcpy(dst, src, aw);
		src += pitch;
		dst += pitch;
	}
}

//update source (!) width in bytes (0-16) (128 original resolution, 8 pixel per byte)
//update source (!) height
static void fb_update_rect_impl(u8 *base, const u8 *src, u8 updateWidth, u8 updateHeight)
{
	u8 srcPitch = 16 - updateWidth;
	while(updateHeight--)
	{
		u8 width = updateWidth;
		u8 *dst = base;
		while(width--)
		{
			u8 srcByte = *src++;
			*dst++ = dup4[srcByte >> 4];
			*dst++ = dup4[srcByte & (u8)0x0f];
		}
		src += srcPitch;
		u8 * next = (u8*)fb_next_line((u16)base);
		memcpy(next, base, updateWidth << 1);
		base = (u8*)fb_next_line((u16)next);
	}
}

void fb_update_attrs(void)
{
	memcpy(VRAM_ATTRS + 0x80, fbAttr, 32 * 16);
}

void fb_update(void) {
	fb_update_attrs();
	fb_update_rect_impl(VRAM_ADDR + 0x80, fbData, 16, 64);
}

void fb_update_rect(u8 x, u8 y, u8 w, u8 h)
{
	u8 r = (x + w + 7) & (u8)0xf8; //align 8 right
	x = x & (u8)0xf8; //align by 8
	w = r - x;
	//fb_update_rect_impl(fb_get_base_addr(x << 1, (y << 1) + 32), FB_BASE_ADDR(x, y), w, h);
	fb_update_rect_impl(fb_get_base_addr(0, (y << 1) + 32), FB_BASE_ADDR(0, y), 16, h);
}

void fb_clear(u8 bg)
{
	border(bg >> 3);
	memset(fbData, 0, sizeof(fbData));
	memset(fbAttr, bg, sizeof(fbAttr));
	fb_update();
}

void fb_clear_attrs(u8 bg)
{
	memset(VRAM_ATTRS, bg, 0x300);
}

void fb_clear_pixels(u8 value)
{
	memset(VRAM_ADDR, value, 0x1800);
}
