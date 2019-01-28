#ifndef TILES_H
#define TILES_H

#include "types.h"
#include "texture.h"

void panel_draw(void);
void banner_draw(Texture *banner, u8 text_id, u8 text_x, u8 text_y);
void room_draw(void);
void room_with_letter(void);

#endif
