#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/extu.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, extu_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0xf0f0);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x51);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x00f0);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
}

TEST_F(CPUTestFix, extu_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0xf0f0f0f0);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x71);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x0000f0f0);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
}
