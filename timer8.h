#ifndef _TIMER8_INCLUDED_
#define _TIMER8_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include <chrono>
#include "interrupt/interrupt_type.h"

// タイマスレッドを管理するクラス、invalidate されない限りは動き続ける
class Looper {
    std::thread* timer_thread;
    int wait_time;
    bool isValid;

    void wait_and_notify()
    {
        while (this->isValid) {
            std::this_thread::sleep_for(std::chrono::milliseconds(this->wait_time));
            if (this->isValid) {
                // interrupt!
            }
        }
    }

public:
    Looper(int wait_time)
        : isValid(true)
        , wait_time(wait_time)
        , timer_thread(new std::thread(&Looper::wait_and_notify, this))
    {}

    ~Looper()
    {
        if (this->timer_thread->joinable()) {
            this->timer_thread->join();
        }
        delete this->timer_thread;
    }

    void invalidate()
    {
        this->isValid = false;
    }

    inline bool joinable() { return this->timer_thread->joinable(); }
    inline void join() { this->timer_thread->join(); }
};


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
    // std::thread* timer_threads[2];
    Looper* loopers[2];
    uint8_t reg[sizeof(TMR)];
    Timer8::TMR* tmr;
    std::condition_variable& interrupt_cv;
    bool interrupt_status[8];

    void loop(uint8_t index);

    uint8_t get_8tcr0_cks() { return this->tmr->tcr0 & 0x07; }
    // double wait_time_for_interrupt_CMIA0();

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
