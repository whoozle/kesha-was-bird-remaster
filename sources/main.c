#include <string.h>
#include "runtime.h"
#include "texture_memory_erizer.h"
#include "texture_fday_device.h"
#include "texture_professor.h"
#include "texture_drinking.h"
#include "texture_room.h"
#include "texture_phone.h"
#include "texture_frame.h"
#include "lz4.h"
#include "fb.h"

void main() {
	gsinit();
	border(7);

	lz4_unpack(fbData, phone_data);
	memset((unsigned char *)0x5800, 7 << 3, 0x300);
	lz4_unpack(fbAttr, phone_attrs);
	fb_update();

	halt();
}
