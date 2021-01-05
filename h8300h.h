#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include "registers/register32.h"
#include "registers/ccr.h"
#include "memory.h"

class H8300H {
private:
    Register32 reg[8];
    CCR ccr;
    unsigned long pc;

public:
    H8300H() : pc(0) {}

    void init();
    void load_elf();
    void step();
    void run();
};

#endif
