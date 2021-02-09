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

// 次の CMIA0 割込み発生までの時間を計算する
double wait_time_for_interrupt_CMIA0(double clock, uint8_t counter, uint8_t tcora0)
{

    return (tcora0 - counter) * clock;
}

/*
ウェイトをやり直すタイミング
- カウンタがセットされたとき
- コンペア対象がセットされたとき
- クロックが切り替えられたとき
*/

/*
とりあえずカスケード、8192分周で動かせるようにする
*/

// new thread([]{wait_for(10); notify_all();})

// 選択したクロックで tcnt をカウントアップし、tcora0 と一致したら割込みを発生させる
// 真面目に1つずつカウントアップしていると重いので、thread_sleep を使う
void Timer8::loop(uint8_t index)
{
    // todo: クロックが開始されるまではループをとめたい
    // キャンセルされたらループを止めたい
    // 割込みを抑制するだけでもいいかもしれない

    // todo: クロックの切り替えを考えると、
    //       sleep_for ではなく wait_for でスリープしたほうがいい
    while (1) {
        // todo: カウンタで設定した時間だけスリープする
        // 内部クロックは20MHzを仮定、あとは TCR::CKS を考慮し秒に変換

        // tcr を見てタイマを調整する
        // 8192 は分周、256は0xffで、timer1がオーバーフローするとtimer0がカウント
        // 1000はミリ秒の調整
        // 20MHz なので、1パルスは 1/20,000,000 = 0.00000005 秒
        // 8192 で分周すると、timer1 が1つカウントするのに
        //   0.00000005 * 8192 = 0.0004096 秒
        // タイマはカスケードしているので、timer0 が1つカウントするのに
        //   0.0004096 * 256 = 0.1048576 秒
        // tcora * 0.1048576 秒だけ wait すればいい

        int wait_in_milli_sec = this->tmr->tcora0 * 0.105 * 1000;

        std::this_thread::sleep_for(std::chrono::milliseconds(wait_in_milli_sec));

        // 割込みを発生させる
        // todo: 仮で CMIA0 を発生させる
        if (index == 0) {
            // todo: どの割り込みを発生させるかはどうやって判定する？
            this->interrupt_status[interrupt_t::CMIA0 - interrupt_t::CMIA0] = true;
            interrupt_cv.notify_all();
        }
    }
}

void Timer8::set_tcr0(uint8_t value)
{
// ここでスレッドを開始する
    uint8_t cks = value & 0x07;
    switch (cks) {
    case 0: // DISCLK
        // 今のタイマスレッドを無効化する
        // todo: looper はどうやって delete する？
        this->loopers[0]->invalidate();
        break;
    case 1: // CLK8
        // 今のタイマスレッドを止めて、新しくスレッドを new する
        this->loopers[0]->invalidate();
        break;
    case 2: // CLK64
        // 今のタイマスレッドを止めて、新しくスレッドを new する
        this->loopers[0]->invalidate();
        break;
    case 3: // CLK8192
        // 今のタイマスレッドを止めて、新しくスレッドを new する
        this->loopers[0]->invalidate();
        break;
    case 4: // カスケード(cnt1 のオーバーフローでカウントする)
        // 今のタイマスレッドを止めて、新しくスレッドを new する
        this->loopers[0]->invalidate();
        // this->loopers[0] = new Looper();
        break;
    case 5: // CLKUP
    case 6: // CLKDOWN
    case 7: // CLKBOTH
        // todo: 外部クロックをサポートする
        break;
    default:
        fprintf(stderr, "Error: Unknown clock select(%d)\n", cks);
        return;
    }

    // todo: 割込みが有効になった場合、タイマをスタート

    this->tmr->tcr0 = value;
}

void Timer8::set_tcr1(uint8_t value)
{
    // todo: set_tcr0 と同じ処理だが、cks=0x4の場合の処理のみ異なる
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
    : loopers { nullptr, nullptr }
    , tmr((TMR*)this->reg)
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
    for (int i = 0; i < 2; i++) {
        this->loopers[i]->invalidate();
        if (this->loopers[i]->joinable()) {
            this->loopers[i]->join();
        }
        delete this->loopers[i];
    }

    // for (int i = 0; i < 2; i++){
    //     if (this->timer_threads[i]) {
    //         if (this->timer_threads[i]->joinable()) {
    //             this->timer_threads[i]->join();
    //         }
    //         delete this->timer_threads[i];
    //     }
    // }
}

// todo: おそらく run は不要
void Timer8::run()
{
    // for (int i = 0; i < 2; i++) {
    //     this->timer_threads[i] = new std::thread(&Timer8::loop, this, i);
    // }

    // printf("Timer8 started\n");
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
    // todo: カウンタが読まれる場合は更新が必要
    return this->reg[address];
}

uint16_t Timer8::read16(uint32_t address)
{
    // todo: カウンタが読まれる場合は更新が必要
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
