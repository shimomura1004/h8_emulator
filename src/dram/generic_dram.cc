#include "generic_dram.h"
#include "util/byteorder.h"
#include <cstdlib>

// TODO: IO ポートの設定が完了するまでは DRAM を読み書きさせない

GenericDRAM::GenericDRAM(uint32_t size)
    : ram((uint8_t*)malloc(sizeof(uint8_t) * size))
{
    if (this->ram == nullptr) {
        fprintf(stderr, "Error: failed to allocate %d bytes DRAM\n", size);
    }
}

GenericDRAM::~GenericDRAM()
{
    if (this->ram != nullptr) {
        free(this->ram);
    }
}

uint8_t GenericDRAM::read8(uint32_t address)
{
    return this->ram[address];
}

uint16_t GenericDRAM::read16(uint32_t address)
{
    return bswap16_if_little_endian(*(uint16_t*)&this->ram[address]);
}

uint32_t GenericDRAM::read32(uint32_t address)
{
    return bswap32_if_little_endian(*(uint32_t*)&this->ram[address]);
}

void GenericDRAM::write8(uint32_t address, uint8_t value)
{
    this->ram[address] = value;
}

void GenericDRAM::write16(uint32_t address, uint16_t value)
{
    *(uint16_t*)&this->ram[address] = bswap16_if_little_endian(value);
}

void GenericDRAM::write32(uint32_t address, uint32_t value)
{
    *(uint32_t*)&this->ram[address] = bswap32_if_little_endian(value);
}

void GenericDRAM::dump(FILE* fp)
{
    fwrite(this->ram, sizeof(uint8_t), 0x200000, fp);
}
