#ifndef _H8300H_TIMER8_INCLUDED_
#define _H8300H_TIMER8_INCLUDED_

#include <cstdint>
#include <condition_variable>
#include "timer8.h"
#include "tmr8.h"

// 8ビットタイマ4つ分
class H8300H_Timer8 : public ITimer8 {
    TMR8 tmr8[4];

public:
    H8300H_Timer8(std::condition_variable& interrupt_cv);

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    // OS 側からは read/write でアクセス
    // 16ビットレジスタとしてアクセスすると2つのタイマを同時に操作できる
    uint8_t read8(uint32_t address, uint8_t channel);
    uint16_t read16(uint32_t address, uint8_t channel);
    void write8(uint32_t address, uint8_t value, uint8_t channel);
    void write16(uint32_t address, uint16_t value, uint8_t channel);

};

#endif
