#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>

#include "registers/register32.h"
#include "registers/ccr.h"
#include "inner_memory.h"

class H8300H {
public:
    Register32 reg[8];
    CCR ccr;
    unsigned long pc;
    InnerMemory memory;

public:
    unsigned char fetch_instruction_byte(unsigned int offset);
    int execute_next_instruction();

public:
    H8300H() : pc(0) {}

    void init();
    void load_elf(std::string filepath);
    int step();
    void run();
    void interrupt();

};

#endif
