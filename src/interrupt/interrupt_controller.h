#ifndef _INTERRUPT_CONTROLLER_INCLUDED_
#define _INTERRUPT_CONTROLLER_INCLUDED_

#include "interrupt_type.h"

class IInterruptController {
public:
    virtual ~IInterruptController() {}

    virtual void set(interrupt_t type) = 0;
    virtual void clear(interrupt_t type) = 0;

    virtual interrupt_t getInterruptType() = 0;
    virtual interrupt_t getTrap() = 0;

};

#endif
