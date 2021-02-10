#include "tmr8.h"

const interrupt_t TMR8::interrupts[] = {
    interrupt_t::CMIA0,
    interrupt_t::CMIB0,
    interrupt_t::CMIA1_CMIB1,
    interrupt_t::TOVI0_TOVI1,
    interrupt_t::CMIA2,
    interrupt_t::CMIB2,
    interrupt_t::CMIA3_CMIB3,
    interrupt_t::TOVI2_TOVI3,
};

void TMR8::set_tcsr_cmfb(bool b)
{
    if (b) {
        this->tcsr |= 0b10000000;
    } else {
        this->tcsr &= 0b01111111;
    }
}

void TMR8::set_tcsr_cmfa(bool b)
{
    if (b) {
        this->tcsr |= 0b01000000;
    } else {
        this->tcsr &= 0b10111111;
    }
}

void TMR8::set_tcsr_ovf(bool b)
{
    if (b) {
        this->tcsr |= 0b00100000;
    } else {
        this->tcsr &= 0b11011111;
    }
}

// todo: tcora のほうが大きい場合は予期せぬ動作となる
double TMR8::get_waittime_for_CMIA() {
    // todo: とりあえずダミーで1000ミリ秒を返す
    return 1000;

    // todo: カスケードなので TMR1 の情報も必要
    return periods[this->clock_kind] * (this->tcora - this->tcnt);
}

double TMR8::get_waittime_for_CMIB() {
    return periods[this->clock_kind] * (this->tcorb - this->tcnt);
}

double TMR8::get_waittime_for_TOVI() {
    return periods[this->clock_kind] * (0xff - this->tcnt);
}

void TMR8::loop(uint8_t index, int waittime, interrupt_t interrupt_type) {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(waittime));
        if (this->valid_clock_id != index) {
            break;
        }

        // todo: interrupts_status ではなく TCSR にビットをセット
        this->interrupt_status[interrupt_type - interrupt_t::CMIA0] = true;
        this->set_interrupt(interrupt_type);
        interrupt_cv.notify_all();
    }
}

void TMR8::set_interrupt(interrupt_t interrupt_type)
{
    if (interrupt_type == interrupt_t::CMIA0) {
        this->set_tcsr_cmfa(true);
    } else if (interrupt_type == interrupt_t::CMIB0) {
        this->set_tcsr_cmfb(true);
    } else if (interrupt_type == interrupt_t::TOVI0_TOVI1) {
        this->set_tcsr_ovf(true);
    }
}

// 過去のタイマを無効にし、新たなタイマを起動する
void TMR8::update_timer() {
    // 過去のタイマを失効させる
    this->valid_clock_id++;

    // 有効にされた割込みに応じて複数のタイマを起動
    if (this->get_tcr_cmieb()) {
        int waittime = get_waittime_for_CMIB();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIB0);
    }
    if (this->get_tcr_cmiea()) {
        int waittime = get_waittime_for_CMIA();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIA0);
    }
    if (this->get_tcr_ovie()) {
        int waittime = get_waittime_for_TOVI();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::TOVI0_TOVI1);
    }
}

// todo: 8TCSR2 は 0x10 にクリアしなければいけない
TMR8::TMR8(std::condition_variable& interrupt_cv)
    : tcr(0x00)
    , tcsr(0xff)
    , tcora(0xff)
    , tcorb(0xff)
    , tcnt(0x00)
    , clock_kind(CLOCK_KIND::ORIG)
    , valid_clock_id(0)
    , interrupt_status{false, false, false, false, false, false, false, false}
    , interrupt_cv(interrupt_cv)
{}

interrupt_t TMR8::getInterrupt()
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (this->interrupt_status[i]) {
            return this->interrupts[i];
        }
    }

    return interrupt_t::NONE;
}

void TMR8::clearInterrupt(interrupt_t type)
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (type == this->interrupts[i]) {
            if (this->interrupt_status[i]) {
                this->interrupt_status[i] = false;
            } else {
                fprintf(stderr, "Error: TMR8 does not generate interruption(%d)\n", type);
            }
        }
    }
}

void TMR8::set_tcr(uint8_t value) {
    this->tcr = value;
    update_timer();
}

void TMR8::set_tcsr(uint8_t value) {
    this->tcsr = value;
    update_timer();
}

void TMR8::set_tcora(uint8_t value) {
    this->tcora = value;
    update_timer();
}

void TMR8::set_tcorb(uint8_t value) {
    this->tcorb = value;
    update_timer();
}

void TMR8::set_tcnt(uint8_t value) {
    this->tcnt = value;
    update_timer();
}
