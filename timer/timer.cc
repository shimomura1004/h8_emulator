#include "timer.h"

const interrupt_t Timer::interrupts[] = {
    interrupt_t::CMIA0,
    interrupt_t::CMIB0,
    interrupt_t::CMIA1_CMIB1,
    interrupt_t::TOVI0_TOVI1,
    interrupt_t::CMIA2,
    interrupt_t::CMIB2,
    interrupt_t::CMIA3_CMIB3,
    interrupt_t::TOVI2_TOVI3,
};

// todo: tcora のほうが大きい場合は予期せぬ動作となる
double Timer::get_waittime_for_CMIA() {
    return periods[this->clock_kind] * (this->tcora - this->tcnt);
}

double Timer::get_waittime_for_CMIB() {
    return periods[this->clock_kind] * (this->tcorb - this->tcnt);
}

double Timer::get_waittime_for_TOVI() {
    return periods[this->clock_kind] * (0xff - this->tcnt);
}

void Timer::loop(uint8_t index, int waittime, interrupt_t interrupt_type) {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(waittime));
        if (this->valid_clock_id != index) {
            break;
        }

        this->interrupt_status[interrupt_type - interrupt_t::CMIA0] = true;
        interrupt_cv.notify_all();
    }
}

// 過去のタイマを無効にし、新たなタイマを起動する
void Timer::update_timer() {
    // 過去のタイマを失効させる
    this->valid_clock_id++;

    // 有効にされた割込みに応じて複数のタイマを起動
    if (this->get_tcr_cmieb()) {
        int waittime = get_waittime_for_CMIB();
        new std::thread(&Timer::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIB0);
    }
    if (this->get_tcr_cmiea()) {
        int waittime = get_waittime_for_CMIA();
        new std::thread(&Timer::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIA0);
    }
    if (this->get_tcr_ovie()) {
        int waittime = get_waittime_for_TOVI();
        new std::thread(&Timer::loop, this, this->valid_clock_id, waittime, interrupt_t::TOVI0_TOVI1);
    }
}

Timer::Timer(std::condition_variable& interrupt_cv)
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

interrupt_t Timer::getInterrupt()
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (this->interrupt_status[i]) {
            return this->interrupts[i];
        }
    }

    return interrupt_t::NONE;
}

void Timer::clearInterrupt(interrupt_t type)
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
