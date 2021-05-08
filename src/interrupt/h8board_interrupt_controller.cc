#include "h8board_interrupt_controller.h"

// 例外処理 1. リセット(最優先)
//         2. 割り込み(さらに外部割込みと内部割込み(内蔵周辺モジュール)にわかれる)
//         3. トラップ命令(プログラム実行状態で常に受け付けられる)
// 割込み禁止状態か判断できるのは H8 のみ
// 割込みコントローラは getInterrupt したときの優先度調停のみ実施

static const interrupt_t external_interrupts[] = {
    NMI,
};

static const interrupt_t timer8_interrupts[] = {
    CMIA0, CMIB0, CMIA1_CMIB1, TOVI0_TOVI1,
    CMIA2, CMIB2, CMIA3_CMIB3, TOVI2_TOVI3,
};

static const interrupt_t sci0_interrupts[] = {
    ERI0, RXI0, TXI0, TEI0,
};
static const interrupt_t sci1_interrupts[] = {
    ERI1, RXI1, TXI1, TEI1,
};
static const interrupt_t sci2_interrupts[] = {
    ERI2, RXI2, TXI2, TEI2,
};

static const interrupt_t traps[] = {
    TRAP0, TRAP1, TRAP2, TRAP3,
};

constexpr static uint8_t external_interrupt_num = sizeof(external_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t timer8_interrupt_num = sizeof(timer8_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t sci0_interrupt_num = sizeof(sci0_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t sci1_interrupt_num = sizeof(sci1_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t sci2_interrupt_num = sizeof(sci2_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t trap_num = sizeof(traps) / sizeof(interrupt_t);

H8BoardInterruptController::H8BoardInterruptController(ISCI& sci0, ISCI& sci1, ISCI& sci2, ITimer8& timer8, INIC& nic)
    : sci0(sci0)
    , sci1(sci1)
    , sci2(sci2)
    , timer8(timer8)
    , nic(nic)
    , interrupt_flag(0)
{}

void H8BoardInterruptController::set(interrupt_t type)
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

void H8BoardInterruptController::clear(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みのクリア
    for (int i = 0; i < external_interrupt_num; i++) {
        if (type == external_interrupts[i]) {
            interrupt_flag &= ~((uint64_t)1 << type);
            return;
        }
    }

    if (type == interrupt_t::IRQ5) {
        this->nic.clearInterrupt(type);
        return;
    }

    // 内部割込み(8ビットタイマ)のクリア
    for (int i = 0; i < timer8_interrupt_num; i++) {
        if (type == timer8_interrupts[i]) {
            this->timer8.clearInterrupt(type);
            return;
        }
    }

    // 内部割込み(SCI)のクリア
    for (int i = 0; i < sci0_interrupt_num; i++) {
        if (type == sci0_interrupts[i]) {
            this->sci0.clearInterrupt(type);
            return;
        }
    }
    for (int i = 0; i < sci1_interrupt_num; i++) {
        if (type == sci1_interrupts[i]) {
            this->sci1.clearInterrupt(type);
            return;
        }
    }
    for (int i = 0; i < sci2_interrupt_num; i++) {
        if (type == sci2_interrupts[i]) {
            this->sci2.clearInterrupt(type);
            return;
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

interrupt_t H8BoardInterruptController::getInterruptType()
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みの確認
    for (int i = 0; i < external_interrupt_num; i++) {
        if (interrupt_flag & (1 << external_interrupts[i])) {
            return external_interrupts[i];
        }
    }

    interrupt_t type = interrupt_t::NONE;

    // IRQ5(Ethernet) の確認
    type = this->nic.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }

    // 内部割込みの確認

    // 8ビットタイマの確認
    type = this->timer8.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }

    // SCI の確認
    type = sci0.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }
    type = sci1.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }
    type = sci2.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }

    return interrupt_t::NONE;
}

interrupt_t H8BoardInterruptController::getTrap()
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
