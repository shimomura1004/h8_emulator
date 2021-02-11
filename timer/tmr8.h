#ifndef _TIMER_TMR_INCLUDED_
#define _TIMER_TMR_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include <chrono>
#include "./common.h"
#include "tcr.h"
#include "tcsr.h"
#include "interrupt/interrupt_type.h"

// 8ビットタイマ
class TMR8 {
    const static interrupt_t interrupts[8];

    TMR8& sub_timer;
    TCR tcr;
    TCSR tcsr;
    uint8_t tcora;
    uint8_t tcorb;
    uint8_t tcnt;

    uint8_t channel;

    uint8_t valid_clock_id;
    bool interrupt_request_flags[8];
    std::condition_variable& interrupt_cv;

    uint8_t get_count_for(interrupt_t type);
    double get_waittime_for(interrupt_t type);

    void loop(uint8_t index, int waittime, interrupt_t interrupt_type);
    void update_timer();
    void set_interrupt(interrupt_t interrupt_type);

public:
    TMR8(uint8_t channel, TMR8& sub_timer, std::condition_variable& interrupt_cv);

    interrupt_t getInterrupt();
    bool clearInterrupt(interrupt_t type);

    uint8_t get_tcr();
    uint8_t get_tcsr();
    uint8_t get_tcora();
    uint8_t get_tcorb();
    uint8_t get_tcnt();

    void set_tcr(uint8_t value);
    void set_tcsr(uint8_t value);
    void set_tcora(uint8_t value);
    void set_tcorb(uint8_t value);
    void set_tcnt(uint8_t value);

};

#endif
