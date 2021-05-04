#ifndef _H8300H_INCLUDED_
#define _H8300H_INCLUDED_

#include <string>
#include <mutex>
#include "cpu/cpu.h"
#include "mcu.h"
#include "sci/sci.h"
#include "interrupt/interrupt_controller.h"
#include "timer/timer8.h"
#include "ioport/ioport.h"
#include "net/nic.h"

// todo: H8300H は CPU の名前、ボードの名前に変える
// H8/3069Fネット対応マイコンLANボード (K-01271)
class H8300H {
public:
    ICPU& cpu;

    // todo: ペリフェラル類の依存関係を整理
    // mcu にしか注入しないオブジェクトが多いはず
    // IDRAM& dram;
    // todo: 参照にしたい
    // todo: interrupt_controller を注入するようにすればここで sci は扱わなくていい
    // ISCI* sci[3];
    // ITimer8& timer8;
    // IOPort& ioport;
    // INIC& nic;
    MCU& mcu;
    IInterruptController& interrupt_controller;

    std::mutex mutex;
    // todo: terminate は不要？
    bool terminate;
    bool is_sleep;
    // スリープ状態から復帰するため、各ペリフェラルに渡して割込み発生時に通知してもらう
    std::condition_variable& interrupt_cv;

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
    H8300H(ICPU& cpu, MCU& mcu, IInterruptController& interrupt_controller);
    ~H8300H();

    void init();
    uint32_t load_elf(std::string filepath);

    bool handle_interrupt();
    int step();

    void print_registers();

    std::mutex& get_mutex() { return mutex; }
};

#endif
