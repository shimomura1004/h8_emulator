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

// todo: リネーム、もしくは Register8/16 を別ファイルに

class Register32 {
    friend class Register16;
    friend class Register8;

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

class Register16 {
    Register32& reg;
    uint8_t index;

public:
    Register16(Register32& reg, uint8_t index) : reg(reg), index(index) {}

    uint16_t get() const {
        return (this->index < 8) ? this->reg.get_r() : this->reg.get_e();
    }
    void set(uint16_t value) {
        (this->index < 8) ? this->reg.set_r(value) : this->reg.set_e(value);
    }

};

class Register8 {
    Register32& reg;
    uint8_t index;

public:
    Register8(Register32& reg, uint8_t index) : reg(reg), index(index) {}

    uint8_t get() const {
        return (this->index < 8) ? this->reg.get_rh() : this->reg.get_rl();
    }
    void set(uint8_t value) {
        (this->index < 8) ? this->reg.set_rh(value) : this->reg.set_rl(value);
    }

};

#endif
