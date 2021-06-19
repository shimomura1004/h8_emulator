#ifndef _CPU_INTERRUPT_TEST_FIXTURE_INCLUDED_
#define _CPU_INTERRUPT_TEST_FIXTURE_INCLUDED_

#include <gtest/gtest.h>
#include "board/h8board.h"
#include "cpu/h83069f.h"
#include "dram/generic_dram.h"
#include "dummy_mcu.h"
#include "sci/dummy_sci.h"
#include "timer/dummy_timer.h"
#include "net/dummy_nic.h"
#include "interrupt/h8board_interrupt_controller.h"

class CPUInterruptControllerTestFix : public ::testing::Test
{
protected:
    H8Board *h8;
    ICPU *cpu;
    IMCU *mcu;
    IDRAM *dram;
    ISCI *scis[3];
    ITimer8 *timer;
    INIC *nic;
    IInterruptController *interrupt_controller;

    void SetUp() override
    {
        this->cpu = new H83069F();
        this->dram = new GenericDRAM(0x2000);
        this->scis[0] = new DummySCI();
        this->scis[1] = new DummySCI();
        this->scis[2] = new DummySCI();
        this->timer = new DummyTimer();
        this->nic = new DummyNIC();
        this->interrupt_controller = new H8BoardInterruptController(*this->scis[0], *this->scis[1], *this->scis[2], *this->timer, *this->nic);
        this->mcu = new DummyMCU(*dram);
        this->h8 = new H8Board(*this->cpu, *this->mcu, *this->interrupt_controller);
        this->h8->init();
    }

    void TearDown() override
    {
        delete this->h8;
        delete this->mcu;
        delete this->interrupt_controller;
        delete this->nic;
        delete this->timer;
        delete this->scis[0];
        delete this->scis[1];
        delete this->scis[2];
        delete this->dram;
        delete this->cpu;
    }
};

#endif
