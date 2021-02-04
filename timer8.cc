#include "timer8.h"
#include <chrono>

// H8/3069F の内部クロックは 20MHz とのこと

const interrupt_t Timer8::interrupts[] = {
    interrupt_t::CMIA0,
    interrupt_t::CMIB0,
    interrupt_t::CMIA1_CMIB1,
    interrupt_t::TOVI0_TOVI1,
    interrupt_t::CMIA2,
    interrupt_t::CMIB2,
    interrupt_t::CMIA3_CMIB3,
    interrupt_t::TOVI2_TOVI3,
};

// 選択したクロックで tcnt をカウントアップし、tcora0 と一致したら割込みを発生させる
// 真面目に1つずつカウントアップしていると重いので、thread_sleep を使う
void Timer8::loop(uint8_t index)
{
    while (1) {
        // todo: カウンタで設定した時間だけスリープする
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 割込みを発生させる
        // todo: 仮で1秒ごとに CMIA0 を発生させる
        if (index == 0) {
            this->interrupt_status[interrupt_t::CMIA0 - interrupt_t::CMIA0] = true;
            interrupt_cv.notify_all();

            // H8_3069F_TMR_TCSR_CMFA
        }
    }
}

void Timer8::set_tcr0(uint8_t value)
{
    // 割込み有効化など
}

void Timer8::set_tcr1(uint8_t value)
{
    // todo: set_tcr0 と同じ処理？
}

static void set_tcsr_cmfb()
{

}

void Timer8::set_tcsr0(uint8_t value)
{
    // uint8_t diff = this->tmr->tcsr0 ^ value;

}

void Timer8::set_tcsr1(uint8_t value)
{

}

Timer8::Timer8(std::condition_variable& interrupt_cv)
    : tmr((TMR*)this->reg)
    , interrupt_cv(interrupt_cv)
    , interrupt_status{false, false, false, false, false, false, false, false}
{
    tmr->tcr0   = 0x00;
    tmr->tcr1   = 0x00;
    // todo: 本当は、8TCSR2 は 0x10 にクリアしなければいけない
    // tmr->tcsr0  = 0x00;
    tmr->tcsr0  = 0xff;
    tmr->tcsr1  = 0x00;
    tmr->tcora0 = 0xff;
    tmr->tcora1 = 0xff;
    tmr->tcorb0 = 0xff;
    tmr->tcorb1 = 0xff;
    tmr->tcnt0  = 0x00;
    tmr->tcnt1  = 0x00;
}

Timer8::~Timer8()
{
    for (int i = 0; i < 2; i++){
        if (this->timer_threads[i]) {
            if (this->timer_threads[i]->joinable()) {
                this->timer_threads[i]->join();
            }
            delete this->timer_threads[i];
        }
    }
}

void Timer8::run()
{
    for (int i = 0; i < 2; i++) {
        this->timer_threads[i] = new std::thread(&Timer8::loop, this, i);
    }

    printf("Timer8 started\n");
}

interrupt_t Timer8::getInterrupt()
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (this->interrupt_status[i]) {
            return this->interrupts[i];
        }
    }

    return interrupt_t::NONE;
}

void Timer8::clearInterrupt(interrupt_t type)
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (type == this->interrupts[i]) {
            if (this->interrupt_status[i]) {
                this->interrupt_status[i] = false;
            } else {
                fprintf(stderr, "Error: Timer does not generate interruption(%d)\n", type);
            }
        }
    }
}

uint8_t Timer8::read8(uint32_t address)
{
    return this->reg[address];
}

uint16_t Timer8::read16(uint32_t address)
{
    perror("not implemented Timer8::read16");
    return 0;
}

void Timer8::write8(uint32_t address, uint8_t value)
{
    switch (address) {
    case 0: return set_tcr0(value);
    case 1: return set_tcr1(value);
    case 2: return set_tcsr0(value);
    case 3: return set_tcsr1(value);
    case 4: case 5: case 6: case 7: case 8: case 9: 
        // tcora0, tcora1, tcorb0, tcorb1, tcnt0, tcnt1 の場合は
        // そのまま代入するだけでいい
        this->reg[address] = value;
        return;
    default:
        fprintf(stderr, "Error: Invalid offset %d for timer8\n", address);
        return;
    }
}

void Timer8::write16(uint32_t address, uint16_t value)
{

}
