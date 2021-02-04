#ifndef _TIMER8_INCLUDED_
#define _TIMER8_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include "interrupt/interrupt_type.h"

// 8ビットタイマ2つ分
// todo: 4つ分にしたほうがいいかも
class Timer8 {
public:
    struct TMR {
        uint8_t tcr0;
        uint8_t tcr1;
        uint8_t tcsr0;
        uint8_t tcsr1;
        uint8_t tcora0;
        uint8_t tcora1;
        uint8_t tcorb0;
        uint8_t tcorb1;
        uint8_t tcnt0;
        uint8_t tcnt1;
    };
    const static interrupt_t interrupts[8];

private:
    std::thread* timer_threads[2];
    uint8_t reg[sizeof(TMR)];
    Timer8::TMR* tmr;
    std::condition_variable& interrupt_cv;
    bool interrupt_status[8];

    void loop(uint8_t index);

    void set_tcr0(uint8_t value);
    void set_tcr1(uint8_t value);
    void set_tcsr0(uint8_t value);
    void set_tcsr1(uint8_t value);

public:
    Timer8(std::condition_variable& interrupt_cv);
    ~Timer8();
    void run();

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    // OS 側からは read/write でアクセス
    // 16ビットレジスタとしてアクセスすると2つのタイマを同時に操作できる
    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);

};

#endif
