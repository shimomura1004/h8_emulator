#ifndef _MCU_INCLUDED_
#define _MCU_INCLUDED_

#include <cstdint>
#include <string>

class IMCU {
public:
    virtual ~IMCU() {}

    virtual void init() = 0;

    virtual uint8_t read8(uint32_t address) = 0;
    virtual uint16_t read16(uint32_t address) = 0;
    virtual uint32_t read32(uint32_t address) = 0;

    virtual void write8(uint32_t address, uint8_t value) = 0;
    virtual void write16(uint32_t address, uint16_t value) = 0;
    virtual void write32(uint32_t address, uint32_t value) = 0;

    virtual uint32_t load_elf(std::string filepath) = 0;
    virtual uint32_t get_vector(uint8_t index) = 0;
    virtual void dump(std::string filepath) = 0;

};

#endif
