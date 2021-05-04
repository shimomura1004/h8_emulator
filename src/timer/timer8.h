#ifndef _TIMER8_INCLUDED_
#define _TIMER8_INCLUDED_

#include <cstdint>
#include "interrupt/interrupt_type.h"

// 8ビットタイマ4つ分
class ITimer8 {
public:
    virtual ~ITimer8() {}

    virtual interrupt_t getInterrupt() = 0;
    virtual void clearInterrupt(interrupt_t type) = 0;

    virtual uint8_t read8(uint32_t address, uint8_t channel) = 0;
    virtual uint16_t read16(uint32_t address, uint8_t channel) = 0;
    virtual void write8(uint32_t address, uint8_t value, uint8_t channel) = 0;
    virtual void write16(uint32_t address, uint16_t value, uint8_t channel) = 0;

};

#endif
