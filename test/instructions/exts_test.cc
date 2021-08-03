#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, exts_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0xf0f0);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0xd1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0xfff0);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, exts_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0xf00f);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0xd1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x000f);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}


TEST_F(CPUTestFix, exts_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0xf0f0f0f0);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0xf1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0xfffff0f0);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, exts_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0xf0f00f0f);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0xf1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x00000f0f);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}
