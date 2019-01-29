#include "font.h"
#include "fb.h"
#include "text.h"
#include "font_font.h"
#include <string.h>
#include "audio.h"

u8 font_draw_glyph(const Glyph *glyph, u8 x, u8 y)
{
	u8 h = glyph->height;
	if (h == 0)
		return FONT_FONT_SPACE_WIDTH;

	y -= glyph->descend;
	u16 base = ((u16)y << 4) + (x >> 3);
	const u8 * data = glyph->data;
	u8 w = glyph->width;

	u8 xp = x & 7;
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
	fb_update_rect(x, y, w, glyph->height);
	return w + 1;
}

u8 font_draw_char(char ch, u8 x, u8 y)
{
	if (ch >= FONT_FONT_CMIN && ch < FONT_FONT_CMAX)
	{
		const Glyph * glyph = font_font + (ch - FONT_FONT_CMIN);
		return font_draw_glyph(glyph, x, y);
	}
	else
		return 0;
}

u8 text_draw_impl(u8 x, u8 y, u8 textId, u8 audio)
{
	const u8 * text = text_index[textId];
	u8 cx = x;

	while(1)
	{
		u8 ch = *text++;
		if (ch == 0)
			break;

		u8 w;
		if (ch == 0xffu)
		{
			y += 8;
			w = 0;
			cx = x;
		}
		else
		{
			w = font_draw_char(ch, cx, y);
			if (w == 0)
				w = FONT_FONT_SPACE_WIDTH;
		}
		cx += w;
		if (audio)
			audio_play_sync(audio_text_beep);
	}
	return cx;
}

u8 text_draw(u8 x, u8 y, u8 text_id)
{ return text_draw_impl(x, y, text_id, 1); }

u8 text_draw_ns(u8 x, u8 y, u8 text_id)
{ return text_draw_impl(x, y, text_id, 0); }
