#include "call_galina.h"
#include "vars.h"
#include "_dialogs.h"
#include "tiles.h"
#include "texture_drinking.h"
#include "runtime.h"

void drinking_draw(void)
{
	texture_draw_fullscreen(&texture_drinking);
	sleep(90);
}

void call_galina(void)
{
	++galina_calls;
	switch(galina_calls)
	{
		case 1:
			dialog_galina_2();
			break;
		case 2:
			dialog_galina_3();
			break;
		case 3:
			dialog_galina_4();
			break;
		case 4:
			dialog_galina_5();
			break;
		default:
			if (pets_got_galina) {
				dialog_no_answer_1();
			} else if (!pets_called) {
				dialog_no_answer_1();
			} else {
				dialog_galina_6();
			}
			break;
	}
}
