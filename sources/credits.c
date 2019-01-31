#include "credits.h"
#include "types.h"
#include "fb.h"
#include "font.h"
#include "text.h"
#include "texture_memory_erizer.h"
#include "texture_professor.h"
#include "texture_earth_4.h"
#include "audio_theme.h"
#include "runtime.h"
#include "audio.h"
#include <string.h>

void credits_page(Texture * texture, u8 text_id, u8 text_x, u8 text_y)
{
	texture_draw_fullscreen(texture);

	u8 b = text_y & (u8)0xf8u;
	if (b > 56)
		b = 56;

	u8 e = b + 8;
	u8 h = e - b;

	u8 * addr = FB_BASE_ADDR(0, b);
	memset(addr, 0, h << 4);
	fb_update_rect(0, b, 16, h);

	memset(fbAttr + FB_ATTR_OFFSET(0, b >> 2), 7, 64);
	fb_update_attrs();
	text_draw_ns(text_x, b + 2, text_id);
	audio_play_music(audio_theme_index, audio_theme_data, 0);
}

void credits(void)
{
	while(1)
	{
		credits_page(&texture_memory_erizer, text_credits_pro, 21, 59);
		credits_page(&texture_professor, text_credits_art, 0, 50);
		credits_page(&texture_earth_4, text_credits_music, 0, 58);
	}
}
