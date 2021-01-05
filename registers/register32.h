#ifndef _REGISTER_32_INCLUDED_
#define _REGISTER_32_INCLUDED_

// リトルエンディアン環境を前提にしている
// レジスタにはビッグエンディアンで格納する

class Register32 {
private:
    unsigned char reg[4];

public:
    unsigned long get_er() { return __builtin_bswap32(*((unsigned long*)reg)); }
    unsigned short get_e() { return __builtin_bswap16(*((unsigned short*)reg)); }
    unsigned short get_r() { return __builtin_bswap16(*((unsigned short*)(&reg[2]))); }
    unsigned char get_rh() { return reg[2]; }
    unsigned char get_rl() { return reg[3]; }

    void set_er(unsigned long value) { *((unsigned long*)reg) = __builtin_bswap32(value); }
    void set_e(unsigned short value) { *((unsigned short*)reg) = __builtin_bswap16(value); }
    void set_r(unsigned short value) { *((unsigned short*)(&reg[2])) = __builtin_bswap16(value); }
    void set_rh(unsigned char value) { reg[2] = value; }
    void set_rl(unsigned char value) { reg[3] = value; }

};

#endif
