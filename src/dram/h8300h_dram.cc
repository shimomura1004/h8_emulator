#include "h8300h_dram.h"

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

// todo: IO ポートの設定が完了するまでは DRAM を読み書きさせない

H8300H_DRAM::H8300H_DRAM()
    : ram {0}
{}

uint8_t H8300H_DRAM::read8(uint32_t address)
{
    return this->ram[address];
}

uint16_t H8300H_DRAM::read16(uint32_t address)
{
    return bswap16_if_little_endian(*(uint16_t*)&this->ram[address]);
}

uint32_t H8300H_DRAM::read32(uint32_t address)
{
    return bswap32_if_little_endian(*(uint32_t*)&this->ram[address]);
}

void H8300H_DRAM::write8(uint32_t address, uint8_t value)
{
    this->ram[address] = value;
}

void H8300H_DRAM::write16(uint32_t address, uint16_t value)
{
    *(uint16_t*)&this->ram[address] = bswap16_if_little_endian(value);
}

void H8300H_DRAM::write32(uint32_t address, uint32_t value)
{
    *(uint32_t*)&this->ram[address] = bswap32_if_little_endian(value);
}

void H8300H_DRAM::dump(FILE* fp)
{
    fwrite(this->ram, sizeof(uint8_t), 0x200000, fp);
}
