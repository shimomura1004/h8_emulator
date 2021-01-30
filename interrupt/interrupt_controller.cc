#include "interrupt_controller.h"
#include "interrupt_type.h"

// 例外処理 1. リセット(最優先)
//         2. 割り込み(さらに外部割込みと内部割込み(内蔵周辺モジュール)にわかれる)
//         3. トラップ命令(プログラム実行状態で常に受け付けられる)
// todo: sci は外部割り込み、SCI レジスタに応じてマスクする必要あり

static const interrupt_t external_interrupts[] = {
    NMI,
    IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5,
};

static const interrupt_t internal_interrupts[] = {
    ERI0, RXI0, TXI0, TEI0,
    ERI1, RXI1, TXI1, TEI1,
    ERI2, RXI2, TXI2, TEI2,
};

static const interrupt_t traps[] = {
    TRAP0, TRAP1, TRAP2, TRAP3,
};

constexpr static uint8_t external_interrupt_num = sizeof(external_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t internal_interrupt_num = sizeof(internal_interrupts) / sizeof(interrupt_t);
constexpr static uint8_t trap_num = sizeof(traps) / sizeof(interrupt_t);

InterruptController::InterruptController(SCI** sci)
    : sci(sci)
    , interrupt_flag(0)
{
}

void InterruptController::set(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);

    // 外部割込みは割込みコントローラからはセットされない
    // 内部割込み(SCI)は割込みコントローラからはセットされない
    // todo: 外部・内部割込みが入ったとき、どうやって CPU を起こす？

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

    // 内部割込み(SCI)のクリア
    for (int i = 0; i < internal_interrupt_num; i++) {
        if (type == internal_interrupts[i]) {
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
            // CPU がスリープ状態のときは起こす
            sleep_cv.notify_all();
            return external_interrupts[i];
        }
    }

    // 内部割込み(SCI)の確認
    interrupt_t type;
    for (int i = 0; i < 3; i++) {
        type = sci[i]->getInterrupt();
        if (type != interrupt_t::NONE) {
            sleep_cv.notify_all();
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
            sleep_cv.notify_all();
            return traps[i];
        }
    }

    return interrupt_t::NONE;
}

void InterruptController::wait_for_interruption()
{
    std::unique_lock<std::mutex> lock(sleep_mutex);
    sleep_cv.wait(lock);
}
