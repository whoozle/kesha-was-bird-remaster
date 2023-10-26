#include "lz4.h"
#include <string.h>

static void copy(const u8 *src, u8 *dst, u16 size) {
    while(size--)
        *dst++ = *src++;
}

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
			copy(src, dst, size);
			dst += size;
			src += size;
		}

		u16 offset = *(const u16 *)src;
		src += 2;

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
		copy(dst - offset, dst, size);
		dst += size;
	}

	return dst;
}
