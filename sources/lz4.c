#include "lz4.h"
#include <string.h>

const unsigned char * lz4_unpack(unsigned char *dst, const unsigned char *src)
{
	u16 usize = *(const u16 *)src;
	src += 4;
	const unsigned char *end = dst + usize;
	while(dst < end) {
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
		offset |= ((u16)*src++) << 8;

		size = 4 + (code & 0x0f);
		if (size == 19)
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
