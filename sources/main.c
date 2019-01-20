#include <string.h>

int main() {
	memset((unsigned char *)0x4000, 0x1, 0x1800);
	memset((unsigned char *)0x5800, 0x07, 0x300);

	while(1);
	return 0;
}
