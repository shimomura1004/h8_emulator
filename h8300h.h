#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>
#include <mutex>

#include "registers/register32.h"
#include "registers/ccr.h"
#include "inner_memory.h"
#include "sci/sci.h"
#include "interrupt/interrupt_controller.h"

class H8300H {
public:
    Register32 reg[8];
    Register32& sp;
    CCR ccr;
    uint32_t pc;
    InnerMemory memory;

    Sci* sci1;
    std::mutex mutex;
    bool terminate;

    InterruptController interrupt_controller;
    bool is_sleep;

public:
    unsigned char fetch_instruction_byte(unsigned int offset);
    int execute_next_instruction();

    void push_to_stack_b(uint8_t value, unsigned int register_index = 7);
    uint8_t pop_from_stack_b(unsigned int register_index = 7);
    void push_to_stack_w(uint16_t value, unsigned int register_index = 7);
    uint16_t pop_from_stack_w(unsigned int register_index = 7);
    void push_to_stack_l(uint32_t value, unsigned int register_index = 7);
    uint32_t pop_from_stack_l(unsigned int register_index = 7);

    void save_pc_and_ccr_to_stack();
    void restore_pc_and_ccr_from_stack();

public:
    H8300H();
    ~H8300H();

    void init();
    uint32_t load_elf(std::string filepath);
    int step();

    void print_registers();

    std::mutex& get_mutex() { return mutex; }
};

#endif
