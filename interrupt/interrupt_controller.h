#ifndef _INTERRUPT_CONTROLLER_INCLUDED_
#define _INTERRUPT_CONTROLLER_INCLUDED_

#include <mutex>
#include <condition_variable>
#include "../sci/sci.h"
#include "interrupt_type.h"

class InterruptController {
    SCI** sci;
    std::mutex mutex;
    uint64_t interrupt_flag;

    std::mutex sleep_mutex;
    std::condition_variable sleep_cv;

public:
    InterruptController(SCI** sci);

    void set(interrupt_t type);
    void clear(interrupt_t type);

    interrupt_t getInterruptType();
    interrupt_t getTrap();
    void wait_for_interruption();

};

#endif
