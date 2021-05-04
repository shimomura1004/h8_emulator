#ifndef _H8300H_TIMER8_INCLUDED_
#define _H8300H_TIMER8_INCLUDED_

#include <cstdint>
#include <condition_variable>
#include "timer8.h"
#include "tmr8.h"

// todo: H8300H に特有の機能はなく、generic な実装としてよい
// 8ビットタイマ4つ分
class H8300H_Timer8 : public ITimer8 {
    TMR8 tmr8[4];

public:
    H8300H_Timer8(std::condition_variable& interrupt_cv);

    interrupt_t getInterrupt() override;
    void clearInterrupt(interrupt_t type) override;

    // OS 側からは read/write でアクセス
    // 16ビットレジスタとしてアクセスすると2つのタイマを同時に操作できる
    uint8_t read8(uint32_t address, uint8_t channel) override;
    uint16_t read16(uint32_t address, uint8_t channel) override;
    void write8(uint32_t address, uint8_t value, uint8_t channel) override;
    void write16(uint32_t address, uint16_t value, uint8_t channel) override;

};

#endif
