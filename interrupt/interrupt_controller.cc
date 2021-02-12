#include "interrupt_controller.h"
#include "interrupt_type.h"

// 例外処理 1. リセット(最優先)
//         2. 割り込み(さらに外部割込みと内部割込み(内蔵周辺モジュール)にわかれる)
//         3. トラップ命令(プログラム実行状態で常に受け付けられる)
// 割込み禁止状態か判断できるのは H8 のみ
// 割込みコントローラは getInterrupt したときの優先度調停のみ実施

static const interrupt_t external_interrupts[] = {
    NMI,
    IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5,
};

static const interrupt_t timer8_interrupts[] = {
    CMIA0, CMIB0, CMIA1_CMIB1, TOVI0_TOVI1,
    CMIA2, CMIB2, CMIA3_CMIB3, TOVI2_TOVI3,
};

static const interrupt_t sci_interrupts[] = {
    ERI0, RXI0, TXI0, TEI0,
    ERI1, RXI1, TXI1, TEI1,
    ERI2, RXI2, TXI2, TEI2,
};

static const interrupt_t traps[] = {
    TRAP0, TRAP1, TRAP2, TRAP3,
};

constexpr static uint8_t external_interrupt_num = sizeof(external_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t timer8_interrupt_num = sizeof(timer8_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t sci_interrupt_num = sizeof(sci_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t trap_num = sizeof(traps) / sizeof(interrupt_t);

InterruptController::InterruptController(SCI** sci, Timer8* timer8)
    : sci(sci)
    , timer8(timer8)
    , interrupt_flag(0)
{}

void InterruptController::set(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みは割込みコントローラからはセットされない
    // 内部割込み(SCI)は割込みコントローラからはセットされない
    // トラップのセット
    for (int i = 0; i < trap_num; i++) {
        if (type == traps[i]) {
            interrupt_flag |= ((uint64_t)1 << type);
            break;
        }
    }
}

void InterruptController::clear(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みのクリア
    for (int i = 0; i < external_interrupt_num; i++) {
        if (type == external_interrupts[i]) {
            interrupt_flag &= ~((uint64_t)1 << type);
            return;
        }
    }

    // 内部割込み(8ビットタイマ)のクリア
    for (int i = 0; i < timer8_interrupt_num; i++) {
        if (type == timer8_interrupts[i]) {
            timer8->clearInterrupt(type);
        }
    }

    // 内部割込み(SCI)のクリア
    for (int i = 0; i < sci_interrupt_num; i++) {
        if (type == sci_interrupts[i]) {
            sci[i / 4]->clearInterrupt(type);
        }
    }

    // トラップのクリア
    for (int i = 0; i < trap_num; i++) {
        if (type == traps[i]) {
            interrupt_flag &= ~((uint64_t)1 << type);
            return;
        }
    }
}

interrupt_t InterruptController::getInterruptType()
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みの確認
    for (int i = 0; i < external_interrupt_num; i++) {
        if (interrupt_flag & (1 << external_interrupts[i])) {
            return external_interrupts[i];
        }
    }

    // 内部割込みの確認
    interrupt_t type = interrupt_t::NONE;

    // 8ビットタイマの確認
    type = timer8->getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }

    // SCI の確認
    for (int i = 0; i < 3; i++) {
        type = sci[i]->getInterrupt();
        if (type != interrupt_t::NONE) {
            return type;
        }
    }

    return interrupt_t::NONE;
}

interrupt_t InterruptController::getTrap()
{
    std::lock_guard<std::mutex> lock(mutex);

    // トラップの確認
    for (int i = 0; i < trap_num; i++) {
        if (interrupt_flag & (1 << traps[i])) {
            return traps[i];
        }
    }

    return interrupt_t::NONE;
}
