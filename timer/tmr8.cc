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

// todo: カウンタ(TCNT)の値を考慮する
uint8_t get_target_for(interrupt_t type)
{

}

double TMR8::get_waittime_for(interrupt_t type)
{
    uint8_t target = 0;
    switch (type) {
    case interrupt_t::CMIA0:
        if (this->channel == 0) {
            target = this->tcora;
        } else {
            fprintf(stderr, "Error: CMIA0 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::CMIB0:
        if (this->channel == 0) {
            target = this->tcorb;
        } else {
            fprintf(stderr, "Error: CMIB0 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::CMIA1_CMIB1:
        if (this->channel == 1) {
            target = std::min(this->tcora, this->tcorb);
        } else {
            fprintf(stderr, "Error: CMIA1/CMIB1 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::TOVI0_TOVI1:
        if (this->channel == 0 || this->channel == 1) {
            target = 0xff;
        } else {
            fprintf(stderr, "Error: TOVI0/TOVI1 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::CMIA2:
        if (this->channel == 2) {
            target = this->tcora;
        } else {
            fprintf(stderr, "Error: CMIA2 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::CMIB2:
        if (this->channel == 2) {
            target = this->tcorb;
        } else {
            fprintf(stderr, "Error: CMIB2 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::CMIA3_CMIB3:
        if (this->channel == 3) {
            target = std::min(this->tcora, this->tcorb);
        } else {
            fprintf(stderr, "Error: CMIA3/CMIB3 isn't supported with TMR%d\n", this->channel);
        }
        break;
    case interrupt_t::TOVI2_TOVI3:
        if (this->channel == 2 || this->channel == 3) {
            target = 0xff;
        } else {
            fprintf(stderr, "Error: TOVI2/TOVI3 isn't supported with TMR%d\n", this->channel);
        }
        break;
    default:
        fprintf(stderr, "Error: Unsupported interruption in TMR%d (%d)\n", this->channel, type);
        break;
    };

    uint8_t cks = this->tcr.get_tcr_cks();
    switch (cks) {
    case 0: // クロック入力を禁止
        return -1;
    case 1: // φ / 8 でカウントし、TCORA と一致したら割込み
        return periods[CLOCK_KIND::DIV8] * target;
    case 2: // φ / 64 でカウント
        return periods[CLOCK_KIND::DIV64] * target;
    case 3: // φ / 8192 でカウント
        return periods[CLOCK_KIND::DIV8192] * target;
    case 4:
        switch (this->channel) {
        case 0: // 8TCNT1 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for(interrupt_t::TOVI0_TOVI1) * target;
        case 1: // 8TCNT0 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for(interrupt_t::CMIA0) * target;
        case 2: // 8TCNT3 のオーバーフローでカウント
            return this->sub_timer.get_waittime_for(interrupt_t::TOVI2_TOVI3) * target;
        case 3: // 8TCNT2 のコンペアマッチ A でカウント
            return this->sub_timer.get_waittime_for(interrupt_t::CMIA2) * target;
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
        int waittime = get_waittime_for(interrupt_t::CMIB0);
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIB0);
    }
    if (this->tcr.get_tcr_cmiea()) {
        int waittime = get_waittime_for(interrupt_t::CMIA0);
        new std::thread(&TMR8::loop, this, this->valid_clock_id, waittime, interrupt_t::CMIA0);
    }
    if (this->tcr.get_tcr_ovie()) {
        int waittime = (this->channel < 2)
            ? get_waittime_for(interrupt_t::TOVI0_TOVI1)
            : get_waittime_for(interrupt_t::TOVI2_TOVI3);
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

uint8_t TMR8::get_tcr()
{
    return this->tcr.get_raw();
}

uint8_t TMR8::get_tcsr()
{
    return this->tcsr.get_raw();
}

uint8_t TMR8::get_tcora()
{
    return this->tcora;
}

uint8_t TMR8::get_tcorb()
{
    return this->tcorb;
}

uint8_t TMR8::get_tcnt()
{
    return this->tcnt;
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
