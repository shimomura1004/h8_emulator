#ifndef _DUMMY_NIC_INCLUDED_
#define _DUMMY_NIC_INCLUDED_

#include "net/nic.h"

class DummyNIC : public INIC {
public:
    void run() override {}
    void terminate() override {}

    interrupt_t getInterrupt() override { return interrupt_t::NONE; }
    void clearInterrupt(interrupt_t type) override {}

    uint8_t dma_read(uint16_t address) override { return 0; }
    void dma_write(uint16_t address, uint8_t value) override {}

    void dump(FILE* fp) override {}

public:
    uint8_t read8(uint32_t address) override { return 0; }
    void write8(uint32_t address, uint8_t value) override {}

};

#endif
