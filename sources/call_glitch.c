#include "calls.h"
#include "vars.h"
#include "_dialogs.h"
#include "texture_head_glitch.h";
#include "audio.h"
#include "runtime.h"

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

static Texture texture_glitch = { 8, 8, 0, 0 };

void glitch_fill()
{
	u8 n = 90;
	while(n--)
	{
		u8 x = rand() & 0x7f;
		u8 y = rand() & 0x3f;
		texture_glitch.height = rand() & 8;
		texture_glitch.data = rand() & 0x1fff;
		texture_draw(&texture_glitch, x, y);
		audio_play_sync(audio_text_beep);
	}
}

static const u8 *voice;
void glitch_voice()
{
	voice = 0; //play ZX ROM
	glitch_voice_next();
}

void glitch_voice_next()
{
	u8 n = 60;
	while(n--)
	{
		audio_play_sync(voice);
		voice += 16;
	}
}
