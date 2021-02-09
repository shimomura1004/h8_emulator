#ifndef _TIMER8_INCLUDED_
#define _TIMER8_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include <chrono>
#include "../interrupt/interrupt_type.h"

typedef enum {
    ORIG = 0,
    DIV8,
    DIV64,
    DIV8192,
} CLOCK_KIND;

// ミリ秒単位
const double periods[] = {
    1.0 / 20000000 * 1000,         // 0.00000005秒(20MHz)
    1.0 / 20000000 * 1000 * 8,     // 0.0000004秒
    1.0 / 20000000 * 1000 * 64,    // 0.0000032秒
    1.0 / 20000000 * 1000 * 8192   // 0.0004096秒
};

// 8ビットタイマ
class Timer {
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

    double get_waittime_for_CMIA();
    double get_waittime_for_CMIB();
    double get_waittime_for_TOVI();

    void loop(uint8_t index, int waittime, interrupt_t interrupt_type);
    void update_timer();

public:
    Timer(std::condition_variable& interrupt_cv);

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    uint8_t get_tcr()   { return this->tcr; }
    uint8_t get_tcsr()  { return this->tcsr; }
    uint8_t get_tcora() { return this->tcora; }
    uint8_t get_tcorb() { return this->tcorb; }
    uint8_t get_tcnt()  { return this->tcnt; }

    void set_tcr(uint8_t value) {
        this->tcr = value;
        update_timer();
    }

    void set_tcsr(uint8_t value) {
        this->tcsr = value;
        update_timer();
    }

    void set_tcora(uint8_t value) {
        this->tcora = value;
        update_timer();
    }

    void set_tcorb(uint8_t value) {
        this->tcorb = value;
        update_timer();
    }

    void set_tcnt(uint8_t value) {
        this->tcnt = value;
        update_timer();
    }

};

#endif
