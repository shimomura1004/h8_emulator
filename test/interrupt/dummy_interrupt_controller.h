#ifndef _DUMMY_INTERRUPT_CONTROLLER_INCLUDED_
#define _DUMMY_INTERRUPT_CONTROLLER_INCLUDED_

#include "interrupt/interrupt_controller.h"

class DummyInterruptController : public IInterruptController {
public:
    void set(interrupt_t type) override {}
    void clear(interrupt_t type) override {}

    interrupt_t getInterruptType() override { return interrupt_t::NONE; }
    interrupt_t getTrap() override { return interrupt_t::NONE; }

};

#endif
