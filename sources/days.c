#include "days.h"
#include "tiles.h"
#include "font_font.h"
#include "fb.h"
#include "_dialogs.h"
#include "text.h"
#include "runtime.h"

u8 day_number[3] = { 0, 0, 1 };

void day_next() {
	day_increment();
	day_intertitle();
}

void day_1(void)
{
	dialog_kesha_1();
	day_next();
}

void day_2(void)
{
	//room_with_letter();
	dialog_galina_1();
	sleep(60);
	day_next();
}

void day_intertitle(void)
{
	panel_draw();
	u8 y = 28;
	u8 x = text_draw(50, y, text_day);
	if (day_number[0])
		x += font_draw_char('0' - CHAR_MIN + day_number[0], x, y);
	if (day_number[1])
		x += font_draw_char('0' - CHAR_MIN + day_number[1], x, y);
	if (day_number[2])
		x += font_draw_char('0' - CHAR_MIN + day_number[2], x, y);
	sleep(180); //intertitle_delay
}

void day_increment(void)
{
	++day_number[2];
	if (day_number[2] == 10)
	{
		day_number[2] = 0;
		++day_number[1];
		if (day_number[1] == 10)
		{
			++day_number[0];
		}
	}
}
