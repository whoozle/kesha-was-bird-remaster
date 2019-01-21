#include "lz4.h"
#include <string.h>

const unsigned char * lz4_unpack(unsigned char *dst, const unsigned char *src)
{
	u16 csize = src[0] | (src[1] << 8);
	src += 4;
	const unsigned char *end = src + csize;
	while(src < end) {
		u8 code = *src++;
		u16 size = code >> 4;
		if (size != 0)
		{
			if (size == 15)
			{
				u8 next;
				do
				{
					next = *src++;
					size += next;
				}
				while(next == 255);
			}
			memcpy(dst, src, size);
			dst += size;
			src += size;
		}

		u16 offset = *src++;
		offset |= *src++ << 8;

		size = 2 + (code & 0x0f);
		if (size == 17)
		{
			u8 next;
			do
			{
				next = *src++;
				size += next;
			}
			while(next == 255);
		}
		memcpy(dst, dst - offset, size);
		dst += size;
	}

	return dst;
}
