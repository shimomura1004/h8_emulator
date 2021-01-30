#ifndef _INTERRUPT_CONTROLLER_INCLUDED_
#define _INTERRUPT_CONTROLLER_INCLUDED_

#include "../sci/sci.h"
#include "interrupt_type.h"

class InterruptController {
    SCI** sci;
    // todo: mutex は不要？
    std::mutex mutex;
    uint64_t interrupt_flag;

public:
    InterruptController(SCI** sci);

    void set(interrupt_t type);
    void clear(interrupt_t type);

    interrupt_t getInterruptType();
    interrupt_t getTrap();

};

#endif
