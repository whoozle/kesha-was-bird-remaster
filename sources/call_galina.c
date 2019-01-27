#include "calls.h"
#include "vars.h"
#include "_dialogs.h"
#include "tiles.h"
#include "runtime.h"
#include "text.h"

#include "texture_drinking.h"
#include "texture_professor.h"

void drinking_draw(void)
{
	texture_draw_fullscreen(&texture_drinking);
	sleep(90);
}

void professor_show_banner(void)
{
	banner_draw(&texture_professor, text_professor, 58, 0);
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
