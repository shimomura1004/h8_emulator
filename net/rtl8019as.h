#ifndef _RTL8019AS_INCLUDED_
#define _RTL8019AS_INCLUDED_

#include <cstdio>
#include <cstdint>

class RTL8019AS {

public:
    RTL8019AS();

    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);

    void dump(FILE* fp);

};

#endif
