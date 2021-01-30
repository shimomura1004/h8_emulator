#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>
#include <mutex>

#include "registers/register32.h"
#include "registers/ccr.h"
#include "mcu.h"
#include "sci/sci.h"
#include "interrupt/interrupt_controller.h"

class H8300H {
public:
    Register32 reg[8];
    Register32& sp;
    Register16 reg16[16];
    Register8 reg8[16];
    CCR ccr;
    uint32_t pc;
    SCI* sci[3];
    MCU mcu;

    std::mutex mutex;
    bool terminate;

    InterruptController interrupt_controller;
    bool is_sleep;

public:
    uint8_t fetch_instruction_byte(uint8_t offset);
    int execute_next_instruction();

    void push_to_stack_b(uint8_t value, uint8_t register_index = 7);
    uint8_t pop_from_stack_b(uint8_t register_index = 7);
    void push_to_stack_w(uint16_t value, uint8_t register_index = 7);
    uint16_t pop_from_stack_w(uint8_t register_index = 7);
    void push_to_stack_l(uint32_t value, uint8_t register_index = 7);
    uint32_t pop_from_stack_l(uint8_t register_index = 7);

    void save_pc_and_ccr_to_stack();
    void restore_pc_and_ccr_from_stack();

public:
    H8300H(bool use_stdio=false);
    ~H8300H();

    void init();
    uint32_t load_elf(std::string filepath);

    bool handle_interrupt();
    int step();

    void print_registers();

    std::mutex& get_mutex() { return mutex; }
};

#endif
