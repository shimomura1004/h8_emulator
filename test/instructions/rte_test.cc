#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/rte.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, rte)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg32(7).set(DummyMCU::area2_start + 0x50);
    this->h8->push_to_stack_l(DummyMCU::area2_start + 0x100);

    this->dram->write8(0, 0x54);
    this->dram->write8(1, 0x70);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 0x100);
}
