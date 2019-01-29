#include "tiles.h"
#include "fb.h"
#include "lz4.h"
#include "font.h"
#include "runtime.h"
#include "texture_letter.h"
#include <string.h>

#include "texture_frame.h"
#include "texture_room.h"

void panel_draw(void)
{
	texture_draw_fullscreen(&texture_frame);
}

void banner_draw(Texture *banner, u8 text_id, u8 text_x, u8 text_y)
{
	texture_draw_fullscreen(banner);
	sleep(50);
	u8 b = text_y & (u8)0xf8u;
	if (b > 56)
		b = 56;

	u8 e = b + 8;

	for(u8 i = b; i < e; ++i)
	{
		u8 * addr = FB_BASE_ADDR(0, i);
		memset(addr, 0, 16);
		fb_update_rect(0, i, 16, 1);
		sleep(2);
	}

	memset(fbAttr + FB_ATTR_OFFSET(0, b >> 2), 7, 64);
	fb_update_attrs();
	text_draw(text_x, b + 2, text_id);
	sleep(200);
}

void room_draw(void)
{
	texture_draw_fullscreen(&texture_room);
}

void room_with_letter(void)
{
	room_draw();
	u8 x = 80;
	u8 y = 30;
	while(1)
	{
		texture_draw(&texture_letter, x, y);
		if (y >= 50)
			break;
		sleep(4);
		texture_draw(&texture_letter, x, y);
		y += 2;
	}
	sleep(90);
}
