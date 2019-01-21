#ifndef KESHA_LZ4_H
#define KESHA_LZ4_H

#include "types.h"

//uncompressed lz4 compressed image, returns dst pos after the last byte
u8 * lz4_unpack(u8 *dst, const u8 * src);

#endif
