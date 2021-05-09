#ifndef _CPU_TEST_FIXTURE_INCLUDED_
#define _CPU_TEST_FIXTURE_INCLUDED_

#include <gtest/gtest.h>
#include "board/h8board.h"
#include "cpu/h83069f.h"
#include "dram/generic_dram.h"
#include "dummy_mcu.h"
#include "interrupt/dummy_interrupt_controller.h"

class CPUTestFix : public ::testing::Test
{
protected:
    H8Board *h8;
    ICPU *cpu;
    IMCU *mcu;
    IDRAM *dram;
    IInterruptController *interrupt_controller;

    void SetUp() override
    {
        this->cpu = new H83069F();
        this->dram = new GenericDRAM();
        this->interrupt_controller = new DummyInterruptController;
        this->mcu = new DummyMCU(*dram);
        this->h8 = new H8Board(*this->cpu, *this->mcu, *this->interrupt_controller);
        this->h8->init();
    }

    void TearDown() override
    {
        delete this->h8;
        delete this->mcu;
        delete this->interrupt_controller;
        delete this->dram;
        delete this->cpu;
    }
};

#endif
