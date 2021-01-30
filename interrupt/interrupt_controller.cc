#include "interrupt_controller.h"
#include "interrupt_type.h"

// 例外処理 1. リセット(最優先)
//         2. 割り込み(さらに外部割込みと内部割込み(内蔵周辺モジュール)にわかれる)
//         3. トラップ命令(プログラム実行状態で常に受け付けられる)
// todo: sci は外部割り込み、SCI レジスタに応じてマスクする必要あり

void InterruptController::set(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);
    interrupt_flag |= ((uint64_t)1 << type);

    // CPU がスリープ状態のときは起こす
    // todo: traps のときは必ず起こす
    //       外部割込みのときは I フラグによって動作が変化する
    sleep_cv.notify_all();
}

void InterruptController::clear(interrupt_t type)
{
    // todo: 内部割込みの場合はすぐにクリアする
    //       外部割込みの場合は、割込みを処理したことを外部機器に伝えて、
    //       外部機器側から割込みフラグを落としてもらう
    //       (SCI は外部割り込み)
    std::lock_guard<std::mutex> lock(mutex);
    interrupt_flag &= ~((uint64_t)1 << type);
}

InterruptController::InterruptController()
    : interrupt_flag(0)
{
}

static interrupt_t interrupts[] = {
    NMI,
    // 外部割込み
    IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5,
    // 内部割込み
    ERI0, RXI0, TXI0, TEI0,
    ERI1, RXI1, TXI1, TEI1,
    ERI2, RXI2, TXI2, TEI2,
};
constexpr uint8_t interrupt_num = sizeof(interrupts) / sizeof(interrupt_t);

interrupt_t InterruptController::getInterruptType()
{
    // todo: 本当は MCU と同じように外部機器に機器にいかないといけない
    std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < interrupt_num; i++) {
        if (interrupt_flag & (1 << interrupts[i])) {
            return interrupts[i];
        }
    }

    return interrupt_t::NONE;
}

static interrupt_t traps[] = {
    TRAP0, TRAP1, TRAP2, TRAP3,
};
constexpr uint8_t trap_num = sizeof(traps) / sizeof(interrupt_t);

interrupt_t InterruptController::getTrap()
{
    std::lock_guard<std::mutex> lock(mutex);

    for (int i = 0; i < trap_num; i++) {
        if (interrupt_flag & (1 << traps[i])) {
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
