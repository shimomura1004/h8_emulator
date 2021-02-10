#ifndef _TMR_INCLUDED_
#define _TMR_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include <chrono>
#include "./common.h"
#include "../interrupt/interrupt_type.h"

// 8ビットタイマ
class TMR8 {
    const static interrupt_t interrupts[8];

    uint8_t tcr;
    uint8_t tcsr;
    uint8_t tcora;
    uint8_t tcorb;
    uint8_t tcnt;

    CLOCK_KIND clock_kind;
    uint8_t valid_clock_id;
    bool interrupt_status[8];
    std::condition_variable& interrupt_cv;

    bool get_tcr_cmieb()   { return  this->tcr & 0b10000000; }
    bool get_tcr_cmiea()   { return  this->tcr & 0b01000000; }
    bool get_tcr_ovie()    { return  this->tcr & 0b00100000; }
    uint8_t get_tcr_cclr() { return (this->tcr & 0b00011000) >> 3; }
    uint8_t get_tcr_cks()  { return  this->tcr & 0b00000111; }

    bool get_tcsr_cmfb()     { return this->tcsr & 0b10000000; }
    bool get_tcsr_cmfa()     { return this->tcsr & 0b01000000; }
    bool get_tcsr_ovf()      { return this->tcsr & 0b00100000; }
    bool get_tcsr_adte_ice() { return this->tcsr & 0b00010000; }
    uint8_t get_tcsr_ois()   { return this->tcsr & 0b00001111; }

    void set_tcsr_cmfb(bool b);
    void set_tcsr_cmfa(bool b);
    void set_tcsr_ovf(bool b);

    double get_waittime_for_CMIA();
    double get_waittime_for_CMIB();
    double get_waittime_for_TOVI();

    void loop(uint8_t index, int waittime, interrupt_t interrupt_type);
    void update_timer();
    void set_interrupt(interrupt_t interrupt_type);

public:
    TMR8(std::condition_variable& interrupt_cv);

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    uint8_t get_tcr()   { return this->tcr; }
    uint8_t get_tcsr()  { return this->tcsr; }
    uint8_t get_tcora() { return this->tcora; }
    uint8_t get_tcorb() { return this->tcorb; }
    uint8_t get_tcnt()  { return this->tcnt; }

    void set_tcr(uint8_t value);
    void set_tcsr(uint8_t value);
    void set_tcora(uint8_t value);
    void set_tcorb(uint8_t value);
    void set_tcnt(uint8_t value);

};

#endif
