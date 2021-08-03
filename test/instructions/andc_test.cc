#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, andc_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.ccr().set(0xff);
    this->dram->write8(0, 0x06);
    this->dram->write8(1, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.ccr().raw(), 0x00);
}

TEST_F(CPUTestFix, andc_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.ccr().set(0xff);
    this->dram->write8(0, 0x06);
    this->dram->write8(1, 0b10101010);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.ccr().raw(), 0b10101010);
}
