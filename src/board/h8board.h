#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>
#include <mutex>
#include "cpu/cpu.h"
#include "mcu/mcu.h"
#include "interrupt/interrupt_controller.h"

// todo: 今ボードが持っている役割は CPU に移すべき
// CPU が MCU 経由でデバイスを操作すればいい

class H8Board {
public:
    ICPU& cpu;
    IMCU& mcu;
    IInterruptController& interrupt_controller;

    std::mutex mutex;
    bool is_sleep;
    // スリープ状態から復帰するため、各ペリフェラルに渡して割込み発生時に通知してもらう
    std::condition_variable& interrupt_cv;

    bool wake_for_debugger_flag;

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
    H8Board(ICPU& cpu, IMCU& mcu, IInterruptController& interrupt_controller);

    void init();
    uint32_t load_elf(std::string filepath);

    bool handle_interrupt();
    int step();

    void print_registers();

    std::mutex& get_mutex() { return mutex; }

    void wake_for_debugger();

};

#endif
