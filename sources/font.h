#ifndef KESHA_FONT_H
#define KESHA_FONT_H

#include "types.h"

struct Glyph
{
	u8 width, height;
	u8 descend;
	u8 * data;
};
typedef struct Glyph Glyph;

#define CHAR_MIN (31)

u8 font_draw_glyph(const Glyph *glyph, u8 x, u8 y);
u8 font_draw_char(char ch, u8 x, u8 y);
void text_draw(u8 x, u8 y, u8 textId);

#endif
