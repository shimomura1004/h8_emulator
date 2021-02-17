#include "dram.h"

// todo: ヘルパ関数としてどこかに定義
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define bswap16_if_little_endian(v) (__builtin_bswap16(v));
#define bswap32_if_little_endian(v) (__builtin_bswap32(v));

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define bswap16_if_little_endian(v) (v)
#define bswap32_if_little_endian(v) (v)

#endif
#endif

DRAM::DRAM()
    : ram {0}
{}

uint8_t DRAM::read8(uint32_t address)
{
    return this->ram[address];
}

uint16_t DRAM::read16(uint32_t address)
{
    return bswap16_if_little_endian(*(uint16_t*)&this->ram[address]);
}

uint32_t DRAM::read32(uint32_t address)
{
    return bswap32_if_little_endian(*(uint32_t*)&this->ram[address]);
}

void DRAM::write8(uint32_t address, uint8_t value)
{
    this->ram[address] = value;
}

void DRAM::write16(uint32_t address, uint16_t value)
{
    *(uint16_t*)&this->ram[address] = bswap16_if_little_endian(value);

}

void DRAM::write32(uint32_t address, uint32_t value)
{
    *(uint32_t*)&this->ram[address] = bswap32_if_little_endian(value);
}

void DRAM::dump(FILE* fp)
{
    fwrite(this->ram, sizeof(uint8_t), 0x200000, fp);
}
