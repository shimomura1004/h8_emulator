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

// todo: get_waittime_* は統一できる
// todo: tcora のほうが大きい場合は予期せぬ動作となる
double TMR8::get_waittime_for_CMIA() {
    uint8_t cks = this->tcr.get_tcr_cks();
    switch (cks) {
    case 0: // クロック入力を禁止
        return -1;
    case 1: // φ / 8 でカウントし、TCORA と一致したら割込み
        return periods[CLOCK_KIND::DIV8] * this->tcora;
    case 2: // φ / 64 でカウント
        return periods[CLOCK_KIND::DIV64] * this->tcora;
    case 3: // φ / 8192 でカウント
        return periods[CLOCK_KIND::DIV8192] * this->tcora;
    case 4:
        switch (this->channel) {
        case 0: // 8TCNT1 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * this->tcora;
        case 1: // 8TCNT0 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * this->tcora;
        case 2: // 8TCNT3 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * this->tcora;
        case 3: // 8TCNT2 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * this->tcora;
        default:
            fprintf(stderr, "Error: Invalid channel (%d)\n", this->channel);
            return -1;
        }
    case 5: // 外部クロック
    case 6: // 外部クロック
    case 7: // 外部クロック
        fprintf(stderr, "Error: External clock is not supported.\n");
        return -1;
    default:
        fprintf(stderr, "Error: Invalid cks (%d)\n", cks);
        return -1;
    }
}

double TMR8::get_waittime_for_CMIB() {
    uint8_t cks = this->tcr.get_tcr_cks();
    switch (cks) {
    case 0: // クロック入力を禁止
        return -1;
    case 1: // φ / 8 でカウントし、TCORB と一致したら割込み
        return periods[CLOCK_KIND::DIV8] * this->tcorb;
    case 2: // φ / 64 でカウント
        return periods[CLOCK_KIND::DIV64] * this->tcorb;
    case 3: // φ / 8192 でカウント
        return periods[CLOCK_KIND::DIV8192] * this->tcorb;
    case 4:
        switch (this->channel) {
        case 0: // 8TCNT1 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * this->tcorb;
        case 1: // 8TCNT0 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * this->tcorb;
        case 2: // 8TCNT3 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * this->tcorb;
        case 3: // 8TCNT2 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * this->tcorb;
        default:
            fprintf(stderr, "Error: Invalid channel (%d)\n", this->channel);
            return -1;
        }
    case 5: // 外部クロック
    case 6: // 外部クロック
    case 7: // 外部クロック
        fprintf(stderr, "Error: External clock is not supported.\n");
        return -1;
    default:
        fprintf(stderr, "Error: Invalid cks (%d)\n", cks);
        return -1;
    }
}

double TMR8::get_waittime_for_TOVI() {
    uint8_t cks = this->tcr.get_tcr_cks();
    switch (cks) {
    case 0: // クロック入力を禁止
        return -1;
    case 1: // φ / 8 でカウントし、TCORA と一致したら割込み
        return periods[CLOCK_KIND::DIV8] * 0xff;
    case 2: // φ / 64 でカウント
        return periods[CLOCK_KIND::DIV64] * 0xff;
    case 3: // φ / 8192 でカウント
        return periods[CLOCK_KIND::DIV8192] * 0xff;
    case 4:
        switch (this->channel) {
        case 0: // 8TCNT1 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * 0xff;
        case 1: // 8TCNT0 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * 0xff;
        case 2: // 8TCNT3 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for_TOVI() * 0xff;
        case 3: // 8TCNT2 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for_CMIA() * 0xff;
        default:
            fprintf(stderr, "Error: Invalid channel (%d)\n", this->channel);
            return -1;
        }
    case 5: // 外部クロック
    case 6: // 外部クロック
    case 7: // 外部クロック
        fprintf(stderr, "Error: External clock is not supported.\n");
        return -1;
    default:
        fprintf(stderr, "Error: Invalid cks (%d)\n", cks);
        return -1;
    }
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
        this->tcsr.set_tcsr_cmfa(true);
    } else if (interrupt_type == interrupt_t::CMIB0) {
        this->tcsr.set_tcsr_cmfb(true);
    } else if (interrupt_type == interrupt_t::TOVI0_TOVI1) {
        this->tcsr.set_tcsr_ovf(true);
    }
}

// 過去のタイマを無効にし、新たなタイマを起動する
void TMR8::update_timer() {
    // 過去のタイマを失効させる
    this->valid_clock_id++;

    // 有効にされた割込みに応じて複数のタイマを起動
    if (this->tcr.get_tcr_cmieb()) {
        int waittime = get_waittime_for_CMIB();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIB0);
    }
    if (this->tcr.get_tcr_cmiea()) {
        int waittime = get_waittime_for_CMIA();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIA0);
    }
    if (this->tcr.get_tcr_ovie()) {
        int waittime = get_waittime_for_TOVI();
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::TOVI0_TOVI1);
    }
}

// todo: 8TCSR2 は 0x10 にクリアしなければいけない
TMR8::TMR8(uint8_t channel, TMR8& sub_timer, std::condition_variable& interrupt_cv)
    : sub_timer(sub_timer)
    , tcora(0xff)
    , tcorb(0xff)
    , tcnt(0x00)
    , channel(channel)
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

bool TMR8::clearInterrupt(interrupt_t type)
{
    for (int i = 0; i < sizeof(this->interrupt_status); i++) {
        if (type == this->interrupts[i]) {
            if (this->interrupt_status[i]) {
                this->interrupt_status[i] = false;
                return true;
            } else {
                fprintf(stderr, "Error: TMR8 does not generate interruption(%d)\n", type);
                return false;
            }
        }
    }

    return false;
}

void TMR8::set_tcr(uint8_t value) {
    this->tcr.set_raw(value);
    update_timer();
}

void TMR8::set_tcsr(uint8_t value) {
    this->tcsr.set_raw(value);
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
