#include <gtest/gtest.h>
#include "cpu_test_fixture.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_register_direct_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->cpu->reg8(1).set(0x12);
    // mov.b r1l,r2l
    this->dram->write8(0, 0x0c);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(2).get();
    EXPECT_EQ(val, 0x12);
}

TEST_F(CPUTestFix, mov_register_direct_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->cpu->reg16(1).set(0x1234);
    // mov.w r1,r2
    this->dram->write8(0, 0x0d);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(2).get();
    EXPECT_EQ(val, 0x1234);
}

TEST_F(CPUTestFix, mov_register_direct_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->cpu->reg32(1).set(0x12345678);
    // mov.l er1,er2
    this->dram->write8(0, 0x0f);
    this->dram->write8(1, 0x92);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(2).get();
    EXPECT_EQ(val, 0x12345678);
}
