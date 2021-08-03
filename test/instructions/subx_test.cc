#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, subx_register_direct_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0x12);
    this->h8->cpu.reg8(2).set(0x34);
    this->h8->cpu.ccr().set_c();
    this->dram->write8(0, 0x1e);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg8(2).get(), 0x34 - 0x12 - 1);
}

TEST_F(CPUTestFix, subx_register_direct_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0x12);
    this->h8->cpu.reg8(2).set(0x34);
    this->h8->cpu.ccr().clear_c();
    this->dram->write8(0, 0x1e);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg8(2).get(), 0x34 - 0x12 - 0);
}
