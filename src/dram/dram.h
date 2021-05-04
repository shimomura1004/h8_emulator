#ifndef _DRAM_INCLUDED_
#define _DRAM_INCLUDED_

#include <cstdio>
#include <cstdint>

class IDRAM {
public:
    virtual uint8_t read8(uint32_t address) = 0;
    virtual uint16_t read16(uint32_t address) = 0;
    virtual uint32_t read32(uint32_t address) = 0;
    virtual void write8(uint32_t address, uint8_t value) = 0;
    virtual void write16(uint32_t address, uint16_t value) = 0;
    virtual void write32(uint32_t address, uint32_t value) = 0;

    virtual void dump(FILE* fp) = 0;

};

#endif
