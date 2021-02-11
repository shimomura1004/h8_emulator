#ifndef _TIMER8_INCLUDED_
#define _TIMER8_INCLUDED_

#include <cstdint>
#include <condition_variable>
#include "../interrupt/interrupt_type.h"
#include "tmr8.h"

// 8ビットタイマ2つ分
// todo: 4つ分にする
class Timer8 {
    // todo: 配列にできるか？
    TMR8 tmr8_0;
    TMR8 tmr8_1;

public:
    Timer8(std::condition_variable& interrupt_cv);

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
