#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>

#include "registers/register32.h"
#include "registers/ccr.h"
#include "inner_memory.h"
#include "interrupt_queue.h"

class H8300H {
public:
    Register32 reg[8];
    Register32& sp;
    CCR ccr;
    unsigned long pc;
    InnerMemory memory;
    InterruptQueue interrupt_queue;

public:
    unsigned char fetch_instruction_byte(unsigned int offset);
    int execute_next_instruction();

    void push_to_stack_w(unsigned short);
    unsigned short pop_from_stack_w();
    void push_to_stack_l(unsigned long);
    unsigned long pop_from_stack_l();
    
    void save_pc_and_ccr_to_stack();

public:
    H8300H() : pc(0), sp(reg[7]) {}

    void init();
    void load_elf(std::string filepath);
    int step();
    void run();
    void interrupt();

};

#endif
