#ifndef _DUMMY_TIMER_INCLUDED_
#define _DUMMY_TIMER_INCLUDED_

#include "timer/timer.h"

class DummyTimer : public ITimer8 {
    interrupt_t getInterrupt() override { return interrupt_t::NONE; }
    void clearInterrupt(interrupt_t type) override {}

    uint8_t read8(uint32_t address, uint8_t channel) override { return 0; }
    uint16_t read16(uint32_t address, uint8_t channel) override { return 0; }
    void write8(uint32_t address, uint8_t value, uint8_t channel) override {}
    void write16(uint32_t address, uint16_t value, uint8_t channel) override {}
};

#endif
