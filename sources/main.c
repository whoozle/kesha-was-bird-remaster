#include <string.h>
#include "runtime.h"

extern const unsigned char splash_data[];
extern const unsigned char splash_attrs[];

extern void gsinit(void);

void main() {
	gsinit();

	memcpy((unsigned char *)0x4000, splash_data, 0x1000);
	memcpy((unsigned char *)0x5800, splash_attrs, 0x200);

	halt();
}
