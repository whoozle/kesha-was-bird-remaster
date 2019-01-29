#ifndef KESHA_FB_H

#include "types.h"

#define VRAM_ADDR ((u8 *)0x4000)
#define VRAM_ATTRS ((u8 *)0x5800)


#define FB_BASE_OFFSET(x, y) (((u16)(y) << 4) | ((x) >> 3))
#define FB_BASE_ADDR(x, y) (fbData + FB_BASE_OFFSET(x, y))

#define FB_PIXEL_TO_ATTR(coord) ((coord) >> 3)
#define FB_ATTR_OFFSET(x, y) (((u16)(y) << 5) | x)
#define FB_ATTR_OFFSET_PIXEL(x, y) FB_ATTR_OFFSET(FB_PIXEL_TO_ATTR(x), FB_PIXEL_TO_ATTR(y))

extern u8 fbData[128 * 64 / 8];
extern u8 fbAttr[32 * 16];

void fb_update(void);
void fb_update_attrs(void);
void fb_update_rect(u8 x, u8 y, u8 w, u8 h);
void fb_clear(u8 bg);
void fb_clear_attrs(u8 bg);
u8 * fb_get_base_addr(u8 x, u8 y);
u8 * fb_get_base_attr_addr(u8 x, u8 y);

#endif
