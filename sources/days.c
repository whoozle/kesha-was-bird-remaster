#include "days.h"
#include "tiles.h"
#include "font_font.h"
#include "fb.h"
#include "_dialogs.h"
#include "text.h"
#include "runtime.h"

u8 day_number[3];

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
	text_draw(50, 28, text_day);
	sleep(180); //intertitle_delay
}

void day_increment(void)
{

}
