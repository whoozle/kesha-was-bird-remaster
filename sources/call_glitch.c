#include "calls.h"
#include "vars.h"
#include "_dialogs.h"
#include "texture_head_glitch.h"
#include "audio.h"
#include "runtime.h"
#include "fb.h"
#include <string.h>

static void reset_effect(void) {
	memset(VRAM_ADDR, 2, 0x1b00);
	sleep(1);
	memset(VRAM_ADDR, 0, 0x1800);
}

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

static const u8 *voice;

void glitch_voice_next(void)
{
	u8 n = 60;
	while(n--)
	{
		audio_play_sync(voice);
		voice += 16;
	}
}

void glitch_voice(void)
{
	voice = 0; //play ZX ROM
	glitch_voice_next();
}

static Texture texture_glitch = { 8, 8, 0, 0 };

void glitch_fill(void)
{
	u8 n = 90;
	while(n--)
	{
		u8 x = rand() & 0x7f;
		if (x > 120)
			x = 120;
		u8 y = rand() & 0x3f;
		if (y > 56)
			y = 56;
		texture_glitch.height = rand() & 8;
		texture_glitch.data = (u8 *)(rand() & 0x1fff);
		texture_draw(&texture_glitch, x, y);
		audio_play_sync(audio_text_beep);
	}
	glitch_voice_next();
	reset_effect();
}
