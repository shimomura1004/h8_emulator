#ifndef _INTERRUPT_CONTROLLER_INCLUDED_
#define _INTERRUPT_CONTROLLER_INCLUDED_

#include "sci/sci.h"
#include "timer/timer8.h"
#include "net/rtl8019as.h"
#include "interrupt_type.h"

class InterruptController {
    ISCI** sci;
    ITimer8* timer8;
    RTL8019AS* rtl8019as;

    // todo: mutex は不要？
    std::mutex mutex;
    uint64_t interrupt_flag;

public:
    InterruptController(ISCI** sci, ITimer8* timer8, RTL8019AS* rtl8019as);

    void set(interrupt_t type);
    void clear(interrupt_t type);

    interrupt_t getInterruptType();
    interrupt_t getTrap();

};

#endif
