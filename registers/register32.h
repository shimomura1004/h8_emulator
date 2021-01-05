#ifndef _REGISTER_32_INCLUDED_
#define _REGISTER_32_INCLUDED_

#include <cstddef>

// リトルエンディアン環境を前提にしている
// レジスタにはビッグエンディアンで格納する

class Register32 {
private:
    unsigned char reg[4];

public:
    uint32_t get_er() { return __builtin_bswap32(*((uint32_t*)reg)); }
    uint16_t get_e() { return __builtin_bswap16(*((uint16_t*)reg)); }
    uint16_t get_r() { return __builtin_bswap16(*((uint16_t*)(&reg[2]))); }
    unsigned char get_rh() { return reg[2]; }
    unsigned char get_rl() { return reg[3]; }

    void set_er(uint32_t value) { *((uint32_t*)reg) = __builtin_bswap32(value); }
    void set_e(uint16_t value) { *((uint16_t*)reg) = __builtin_bswap16(value); }
    void set_r(uint16_t value) { *((uint16_t*)(&reg[2])) = __builtin_bswap16(value); }
    void set_rh(unsigned char value) { reg[2] = value; }
    void set_rl(unsigned char value) { reg[3] = value; }

    const unsigned char* raw() const { return reg; }
};

#endif
