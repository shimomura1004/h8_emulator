#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/rotl.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, rotl_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b01010101);
    this->h8->cpu.ccr().set_c();
    this->dram->write8(0, 0x12);
    this->dram->write8(1, 0x81);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b10101010);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, rotl_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b10101010);
    this->h8->cpu.ccr().set_c();
    this->dram->write8(0, 0x12);
    this->dram->write8(1, 0x81);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b01010101);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

