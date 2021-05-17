#ifndef _REGISTER_32_INCLUDED_
#define _REGISTER_32_INCLUDED_

#include <cstddef>
#include "util/byteorder.h"

class Register32 {
    friend class Register16;
    friend class Register8;

private:
    unsigned char reg[4];

    uint16_t get_e() const { return bswap16_if_little_endian(*((uint16_t*)reg)); }
    uint16_t get_r() const { return bswap16_if_little_endian(*((uint16_t*)(&reg[2]))); }
    unsigned char get_rh() const { return reg[2]; }
    unsigned char get_rl() const { return reg[3]; }

    void set_e(uint16_t value) { *((uint16_t*)reg) = bswap16_if_little_endian(value); }
    void set_r(uint16_t value) { *((uint16_t*)(&reg[2])) = bswap16_if_little_endian(value); }
    void set_rh(unsigned char value) { reg[2] = value; }
    void set_rl(unsigned char value) { reg[3] = value; }

public:
    uint32_t get() const { return bswap32_if_little_endian(*((uint32_t*)reg)); }
    void set(uint32_t value) { *((uint32_t*)reg) = bswap32_if_little_endian(value); }

    const unsigned char* raw() const { return reg; }
};

#endif
