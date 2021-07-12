#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/shll.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, shll_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b10101010);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b01010100);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, shll_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b01010101);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b10101010);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}


TEST_F(CPUTestFix, shll_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0b1010101010101010);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x11);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0b0101010101010100);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, shll_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0b0101010101010101);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x11);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0b1010101010101010);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}


TEST_F(CPUTestFix, shll_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0b10101010101010101010101010101010);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x31);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0b01010101010101010101010101010100);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, shll_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0b01010101010101010101010101010101);
    this->dram->write8(0, 0x10);
    this->dram->write8(1, 0x31);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0b10101010101010101010101010101010);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}
