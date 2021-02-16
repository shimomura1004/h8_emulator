#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>
#include <mutex>
#include <condition_variable>
#include "registers/h8register.h"
#include "registers/ccr.h"
#include "mcu.h"
#include "sci/sci.h"
#include "interrupt/interrupt_controller.h"
#include "timer/timer8.h"
#include "ioport/ioport.h"

class H8300H {
public:
    Register32 reg[8];
    Register32& sp;
    Register16 reg16[16];
    Register8 reg8[16];
    CCR ccr;
    uint32_t pc;

    // todo: ペリフェラル類の依存関係を整理
    // mcu が他のペリフェラルをコントローラする感じにできないか？
    SCI* sci[3];
    Timer8 *timer8;
    IOPort *ioport;
    MCU mcu;
    InterruptController interrupt_controller;

    std::mutex mutex;
    // todo: terminate は不要？
    bool terminate;
    bool is_sleep;
    // スリープ状態から復帰するため、SCI に渡して割込み発生時に通知してもらう
    std::condition_variable interrupt_cv;

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
