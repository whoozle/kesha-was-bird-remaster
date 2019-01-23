#include "tiles.h"
#include "fb.h"
#include "lz4.h"
#include "texture_frame.h"

void panel_draw(void)
{
	texture_draw_fullscreen(&texture_frame);
}
