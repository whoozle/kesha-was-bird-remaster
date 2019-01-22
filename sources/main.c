#include <string.h>
#include "runtime.h"
#include "texture_memory_erizer.h"
#include "texture_fday_device.h"
#include "texture_professor.h"
#include "lz4.h"
#include "fb.h"

void main() {
	gsinit();

	lz4_unpack(fbData, professor_data);
	memset((unsigned char *)0x5800, 7, 0x300);
	lz4_unpack(fbAttr, professor_attrs);
	fb_update();

	halt();
}
