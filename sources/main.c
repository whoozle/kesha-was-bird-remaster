#include <string.h>

extern unsigned char splash_data[];

extern void gsinit(void);

int main() {
	gsinit();
	unsigned char * src = splash_data;
	memcpy((unsigned char *)0x4000, src, 0x1800);
	memcpy((unsigned char *)0x5800, src + 0x1800, 0x200);

	while(1);
	return 0;
}
