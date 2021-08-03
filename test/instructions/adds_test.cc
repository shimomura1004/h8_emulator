#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, adds_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x0100);
    this->dram->write8(0, 0x0b);
    this->dram->write8(1, 0x01);

    this->h8->cpu.reg32(2).set(0x0200);
    this->dram->write8(2, 0x0b);
    this->dram->write8(3, 0x82);

    this->h8->cpu.reg32(3).set(0x0300);
    this->dram->write8(4, 0x0b);
    this->dram->write8(5, 0x93);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x0100 + 0x0001);

    ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg32(2).get(), 0x0200 + 0x0002);

    ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg32(3).get(), 0x0300 + 0x0004);
}
