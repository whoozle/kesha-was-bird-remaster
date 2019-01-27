#include "calls.h"
#include "text.h"
#include "tiles.h"
#include "vars.h"
#include "_dialogs.h"

#include "texture_ninja.h"
#include "texture_memory_erizer.h"
#include "texture_fday_device.h"

void ninja_show_banner(void)
{
	banner_draw(&texture_ninja, text_ninja, 33, 30);
}

void ninja_show_eraser(void)
{
	texture_draw_fullscreen(&texture_memory_erizer);
}

void ninja_show_fday_device(void)
{
	banner_draw(&texture_fday_device, text_fday_device, 33, 54);
}

void call_ninja(void)
{
	u8 first_time = ninja_called == 0;
	set_flag(&ninja_called);
	if (pets_got_galina)
	{
		dialog_ninja_3();
	}
	else if (first_time)
	{
		dialog_ninja_1();
	}
	else
	{
		dialog_ninja_2();
	}
}
