#ifndef _UTIL_BYTEORDER_INCLUDED_
#define _UTIL_BYTEORDER_INCLUDED_

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define bswap16_if_little_endian(v) (__builtin_bswap16(v));
#define bswap32_if_little_endian(v) (__builtin_bswap32(v));

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define bswap16_if_little_endian(v) (v)
#define bswap32_if_little_endian(v) (v)

#endif
#endif

#endif
