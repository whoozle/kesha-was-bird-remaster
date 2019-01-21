#include <string.h>
#include "runtime.h"
#include "texture_splash.h"

void main() {
	gsinit();

	const unsigned char * src = splash_data;
	memcpy((unsigned char *)0x4000, src, 0x1000);
	memcpy((unsigned char *)0x5800, src + SPLASH_ATTRS_OFFSET, 0x200);

	halt();
}
