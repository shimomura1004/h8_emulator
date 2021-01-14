#include "interrupt_controller.h"

void InterruptController::set(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);
    interrupt_flag |= ((uint64_t)1 << type);

    // CPU がスリープ状態のときは起こす
    sleep_cv.notify_all();
}

void InterruptController::clear(interrupt_t type)
{
    std::lock_guard<std::mutex> lock(mutex);
    interrupt_flag &= ~((uint64_t)1 << type);
}

InterruptController::InterruptController()
    : interrupt_flag(0)
{
}

bool InterruptController::hasInterrupt()
{
    std::lock_guard<std::mutex> lock(mutex);
    return interrupt_flag;
}

interrupt_t InterruptController::getInterruptType()
{
    std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < 64; i++) {
        if (interrupt_flag & ((uint64_t)1 << i)) {
            return (interrupt_t)i;
        }
    }

    return interrupt_t::NONE;
}

void InterruptController::wait_for_interruption()
{
    std::unique_lock<std::mutex> lock(sleep_mutex);
    sleep_cv.wait(lock);
}
