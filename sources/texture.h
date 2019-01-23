#ifndef KESHA_TEXTURE_H
#define KESHA_TEXTURE_H

#include "types.h"

struct Texture
{
	const u8 *	data;
	const u8 *	attrs;
	u8 			background;
};
typedef struct Texture Texture;

void texture_draw_fullscreen(const Texture *texture);


#endif
