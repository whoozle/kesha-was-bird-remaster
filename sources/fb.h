#ifndef KESHA_FB_H

#include "types.h"

extern u8 fbData[128 * 64 / 4];
extern u8 fbAttr[32 * 16];

extern void fb_update(void);

#endif
