#include "texture.h"
#include "fb.h"
#include "runtime.h"
#include "lz4.h"
#include <string.h>

void texture_draw_fullscreen(const Texture *texture)
{
	border(texture->background);
	fb_clear_attrs(texture->background << 3);
	lz4_unpack(fbData, texture->data);
	lz4_unpack(fbAttr, texture->attrs);
	fb_update();
}
