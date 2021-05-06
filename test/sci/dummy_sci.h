#ifndef _DUMMY_SCI_INCLUDED_
#define _DUMMY_SCI_INCLUDED_

#include "sci/sci.h"

class DummySCI : public ISCI {
    void run() override {}
    void terminate() override {}

    interrupt_t getInterrupt() override { return interrupt_t::NONE; }
    void clearInterrupt(interrupt_t type) override {}

    void dump(FILE* fp) override {}

public:
    uint8_t read(uint32_t address) override { return 0; }
    void write(uint32_t address, uint8_t value) override {}

};

#endif
