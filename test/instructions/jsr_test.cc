#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, jsr_register_indirect)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x100);
    this->h8->cpu.sp().set(DummyMCU::area2_start + 0x200);
    this->dram->write8(0, 0x5d);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 0x100);
    EXPECT_EQ(this->h8->pop_from_stack_l(), DummyMCU::area2_start + 2);
}

TEST_F(CPUTestFix, jsr_absolute_address)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // area2_start = 0x400000
    this->h8->cpu.sp().set(DummyMCU::area2_start + 0x200);
    this->dram->write8(0, 0x5e);
    this->dram->write8(1, 0x40);
    this->dram->write8(2, 0x01);
    this->dram->write8(3, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 0x100);
    EXPECT_EQ(this->h8->pop_from_stack_l(), DummyMCU::area2_start + 4);
}
