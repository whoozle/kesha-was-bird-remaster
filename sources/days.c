#include "days.h"
#include "tiles.h"
#include "font_font.h"
#include "fb.h"

void day_1(void)
{
	panel_draw();
	font_draw_glyph(font_font + ('W' - 31 - 2), 60, 28);
	fb_update();
}

void day_2(void)
{
	panel_draw();
}
