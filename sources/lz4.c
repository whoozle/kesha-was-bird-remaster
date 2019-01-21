#include "lz4.h"

unsigned char * lz4_unpack(unsigned char *dst, const unsigned char *src)
{
	int csize = *src++;
	csize |= (*src++ << 8);
	unsigned char *end = src + csize;
	while(src < end) {
		
	}
	
	return dst;
}
