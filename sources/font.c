#include "font.h"
#include "fb.h"
#include <string.h>

void font_draw_glyph(const Glyph *glyph, u8 x, u8 y)
{
	u16 base = ((u16)y << 4) + (x >> 3);
	u8 w = glyph->width, h = glyph->height;
	y -= glyph->descend;
	u8 xp = x & 7;

	const u8 * src = glyph->data;
	if (xp + w <= 8)
	{
		while(h--)
		{
			fbData[base] ^= (*data++ >> xp);
			base += 16;
		}
	}
	else
	{
		while(h--)
		{
			u8 line = *data++;
			fbData[base++] ^= (line >> xp);
			fbData[base] ^= (line << (8 - xp));
			base += 15;
		}
	}
}
