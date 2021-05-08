#ifndef _GENERAL_INTERRUPT_CONTROLLER_INCLUDED_
#define _GENERAL_INTERRUPT_CONTROLLER_INCLUDED_

#include "interrupt_controller.h"
#include "sci/sci.h"
#include "timer/timer8.h"
#include "net/nic.h"

// todo: 本来 SCI/Timer/NIC からの割り込みは InterruptController が取りまとめるべきでは？
class GeneralInterruptController : public IInterruptController {
    // todo: 参照で受け取りたい
    ISCI** sci;
    ITimer8& timer8;
    INIC& nic;

    // todo: mutex は不要？
    std::mutex mutex;
    uint64_t interrupt_flag;

public:
    GeneralInterruptController(ISCI** sci, ITimer8& timer8, INIC& nic);

    void set(interrupt_t type) override;
    void clear(interrupt_t type) override;

    interrupt_t getInterruptType() override;
    interrupt_t getTrap() override;

};

#endif
