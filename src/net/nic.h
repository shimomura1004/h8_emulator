#ifndef _INIC_INCLUDED_
#define _INIC_INCLUDED_

#include <cstdint>
#include <condition_variable>
#include "interrupt/interrupt_type.h"

class INIC {
public:
    virtual ~INIC() {}

    virtual void run() = 0;
    virtual void terminate() = 0;

    virtual interrupt_t getInterrupt() = 0;
    virtual void clearInterrupt(interrupt_t type) = 0;

    virtual uint8_t dma_read(uint16_t address) = 0;
    virtual void dma_write(uint16_t address, uint8_t value) = 0;

    virtual void dump(FILE* fp) = 0;

public:
    virtual uint8_t read8(uint32_t address) = 0;
    virtual void write8(uint32_t address, uint8_t value) = 0;

};

#endif
