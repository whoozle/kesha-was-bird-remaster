#include <string.h>
#include "runtime.h"
#include "texture_splash.h"
#include "lz4.h"

void main() {
	gsinit();

	lz4_unpack((unsigned char *)0x4000, splash_data);
	lz4_unpack((unsigned char *)0x5800, splash_attrs);

	halt();
}
