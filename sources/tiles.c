#include "tiles.h"
#include "fb.h"
#include "lz4.h"
#include "texture_frame.h"
#include "font.h"
#include "runtime.h"

void panel_draw(void)
{
	texture_draw_fullscreen(&texture_frame);
}

void banner_draw(Texture *banner, u8 text_x, u8 text_y, u8 text_id)
{
	texture_draw_fullscreen(banner);
	sleep(50);
	text_draw(text_x, text_y, text_id);
	sleep(200);
}
