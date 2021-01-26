#ifndef _REGISTER_32_INCLUDED_
#define _REGISTER_32_INCLUDED_

#include <cstddef>

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define bswap16_if_little_endian(v) (__builtin_bswap16(v));
#define bswap32_if_little_endian(v) (__builtin_bswap32(v));

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define bswap16_if_little_endian(v) (v)
#define bswap32_if_little_endian(v) (v)

#endif
#endif

// todo: Register16/Register8 も作ったほうがよさそう

class Register32 {
private:
    unsigned char reg[4];

public:
    uint32_t get_er() const { return bswap32_if_little_endian(*((uint32_t*)reg)); }
    uint16_t get_e() const { return bswap16_if_little_endian(*((uint16_t*)reg)); }
    uint16_t get_r() const { return bswap16_if_little_endian(*((uint16_t*)(&reg[2]))); }
    unsigned char get_rh() const { return reg[2]; }
    unsigned char get_rl() const { return reg[3]; }

    void set_er(uint32_t value) { *((uint32_t*)reg) = bswap32_if_little_endian(value); }
    void set_e(uint16_t value) { *((uint16_t*)reg) = bswap16_if_little_endian(value); }
    void set_r(uint16_t value) { *((uint16_t*)(&reg[2])) = bswap16_if_little_endian(value); }
    void set_rh(unsigned char value) { reg[2] = value; }
    void set_rl(unsigned char value) { reg[3] = value; }

    const unsigned char* raw() const { return reg; }
};

#endif
