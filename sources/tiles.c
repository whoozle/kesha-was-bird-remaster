#include "tiles.h"
#include "fb.h"
#include "lz4.h"
#include "font.h"
#include "runtime.h"
#include "texture_letter.h"

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
	text_draw(text_x, text_y, text_id);
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
