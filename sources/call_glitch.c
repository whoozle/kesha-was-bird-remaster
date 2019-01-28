#include "calls.h"
#include "vars.h"
#include "_dialogs.h"

u8 call_glitch(void)
{
	if (ninja_called && !glitch_1_shown)
	{
		//show_glitch_1
		set_flag(&glitch_1_shown);
		dialog_glitch_1();
		return 1;
	}
	else if (pets_got_galina && !glitch_2_shown)
	{
		set_flag(&glitch_2_shown);
		dialog_glitch_2();
		return 1;
	}
	else
		return 0;
}
