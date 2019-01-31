#include "calls.h"
#include "text.h"
#include "tiles.h"
#include "vars.h"
#include "phone.h"
#include "runtime.h"
#include "audio.h"
#include "fb.h"
#include "font.h"
#include "credits.h"
#include "_dialogs.h"
#include <string.h>

#include "texture_ninja.h"
#include "texture_memory_erizer.h"
#include "texture_fday_device.h"
#include "texture_fish_army.h"
#include "texture_ninja_kesha_1.h"
#include "texture_ninja_kesha_2.h"
#include "texture_ninja_kesha_3.h"
#include "texture_ninja_fish.h"
#include "texture_fish.h"
#include "texture_earth_1.h"
#include "texture_earth_2.h"
#include "texture_earth_3.h"
#include "texture_earth_4.h"

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
	banner_draw(&texture_fday_device, text_fday_device, 30, 54);
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

static void ninja_sword(void)
{
	audio_play_sync(audio_sword_1);
	audio_play_sync(audio_sword_2);
}

static void frame(Texture * bg)
{
	texture_draw_fullscreen(bg);
	ninja_sword();
	sleep(50);
}

void shake_up_down()
{
	u8 n = 60;
	while(n--)
	{
		memmove(VRAM_ADDR + 0x20, VRAM_ADDR, 0x17e0u);
		audio_play_sync(audio_noise);
		memmove(VRAM_ADDR, VRAM_ADDR + 0x20, 0x17e0u);
		audio_play_sync(audio_noise);
	}
}

void shake_left_right()
{
	u8 n = 40;
	while(n--)
	{
		memmove(VRAM_ADDR + 1, VRAM_ADDR, 0x17ffu);
		audio_play_sync(audio_noise);
		memmove(VRAM_ADDR, VRAM_ADDR + 1, 0x17ffu);
		audio_play_sync(audio_noise);
	}
}

static void frame_planet(Texture * bg)
{
	texture_draw_fullscreen(bg);
	shake_left_right();
	audio_play_sync((const u8 *)&ninja_sword);
}


void ninja_fight(void)
{
	dialog_ninja_4();
	u8 key;
	do
	{
		dialog_ninja_5();
		key = read_digit();
		if (key == 1)
		{
			texture_draw_fullscreen(&texture_ninja);
			sleep(100);
			dialog_ninja_66();
		}
	}
	while(key != 2);
	frame(&texture_ninja_fish);
	frame(&texture_ninja_kesha_1);
	frame(&texture_ninja_kesha_2);
	frame(&texture_ninja_kesha_3);
	sleep(100);

	texture_draw_fullscreen(&texture_fish_army);
	shake_up_down();
	texture_draw_fullscreen(&texture_fish);
	shake_up_down();

	{
		u8 n = 24;
		const u16 step = (u16)0x100;
		const u16 size = (u16)0x1800 - step;
		while(n--)
		{
			memmove(VRAM_ADDR, VRAM_ADDR + step, size);
			memset(VRAM_ADDR + size, 0, step);
			audio_play_sync(audio_noise);
			memmove(VRAM_ATTRS, VRAM_ATTRS + 0x20, 0x2e0);
		}
	}

	frame_planet(&texture_earth_1);
	frame_planet(&texture_earth_2);
	frame_planet(&texture_earth_3);
	frame_planet(&texture_earth_4);
	{
		u8 n = 40;
		while(n--)
		{
			u8 r = 192;
			u8 * data = VRAM_ADDR;
			while(r--)
			{
				memcpy(data, data + 1, 31);
				data += 31;
				*data++ = 0;
			}
			r = 24;
			data = VRAM_ATTRS;
			while(r--)
			{
				memcpy(data, data + 1, 31);
				data += 31;
				*data++ = 0;
			}
			audio_play_sync(audio_noise);
		}
	}
	sleep(50);
	border(0);
	fb_clear(7);
	text_draw(38, 30, text_the_end);
	sleep(150);
	font_draw_char(32 /*?*/, 89, 29);
	audio_play_sync(audio_text_beep);
	sleep(50);

	u8 n = 100;
	while(n--)
	{
		u8 x = rand() & (u8)0x7fu;
		if (x > 124)
			x = 124;
		u8 y = rand() & (u8)0x3fu;
		if (y > 58)
			x = 58;
		font_draw_char(32 /*?*/, x, y);
		audio_play_sync(audio_text_beep);
	}

	credits();
}
