#include <gtest/gtest.h>
#include "cpu_interrupt_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/trapa.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUInterruptControllerTestFix, trapa)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->dram->write8(0, 0x57);
    this->dram->write8(1, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);

    interrupt_t type = this->interrupt_controller->getTrap();
    EXPECT_EQ(type, interrupt_t::TRAP0);
}
