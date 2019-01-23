#include "tiles.h"
#include "fb.h"
#include "lz4.h"
#include "texture_frame.h"

void draw_fullscreen_tile(const u8 * data, const u8 *attrs)
{
	lz4_unpack(fbData, data);
	lz4_unpack(fbAttr, attrs);
	fb_update();
}

void panel_draw(void)
{
	draw_fullscreen_tile(frame_data, frame_attrs);
}
