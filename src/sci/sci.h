#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <queue>
#include "interrupt/interrupt_type.h"

class ISCI {
public:
    virtual ~ISCI() {}

    virtual void run() = 0;
    virtual void terminate() = 0;

    virtual interrupt_t getInterrupt() = 0;
    virtual void clearInterrupt(interrupt_t type) = 0;

    virtual void dump(FILE* fp) = 0;

public:
    virtual uint8_t read(uint32_t address) = 0;
    virtual void write(uint32_t address, uint8_t value) = 0;

};

#endif
