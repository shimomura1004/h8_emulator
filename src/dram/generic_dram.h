#ifndef _H8300H_DRAM_INCLUDED_
#define _H8300H_DRAM_INCLUDED_

#include <cstdio>
#include <cstdint>

#include "dram.h"

class GenericDRAM : public IDRAM {
    uint8_t* ram;

public:
    GenericDRAM(uint32_t size);
    ~GenericDRAM();

    uint8_t read8(uint32_t address) override;
    uint16_t read16(uint32_t address) override;
    uint32_t read32(uint32_t address) override;
    void write8(uint32_t address, uint8_t value) override;
    void write16(uint32_t address, uint16_t value) override;
    void write32(uint32_t address, uint32_t value) override;

    void dump(FILE* fp) override;

};

#endif
