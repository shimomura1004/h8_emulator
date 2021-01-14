#ifndef _INTERRUPT_CONTROLLER_INCLUDED_
#define _INTERRUPT_CONTROLLER_INCLUDED_

#include <mutex>
#include <condition_variable>
#include "interrupt_type.h"

class InterruptController {
    std::mutex mutex;
    uint64_t interrupt_flag;

    std::mutex sleep_mutex;
    std::condition_variable sleep_cv;


public:
    InterruptController();

    void set(interrupt_t type);
    void clear(interrupt_t type);

    bool hasInterrupt();
    interrupt_t getInterruptType();
    void wait_for_interruption();

};

#endif
