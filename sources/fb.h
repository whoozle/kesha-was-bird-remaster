#ifndef KESHA_FB_H

#include "types.h"

#define VRAM_ADDR ((u8 *)0x4000)
#define VRAM_ATTRS ((u8 *)0x5800)

extern u8 fbData[128 * 64 / 4];
extern u8 fbAttr[32 * 16];

extern void fb_update(void);
extern void fb_clear_attrs(u8 bg);

#endif
