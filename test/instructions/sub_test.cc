#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, sub_register_direct_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0x12);
    this->h8->cpu.reg8(2).set(0x34);
    this->dram->write8(0, 0x18);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg8(2).get(), 0x34 - 0x12);
}

TEST_F(CPUTestFix, sub_register_direct_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(2).set(0x1234);
    this->h8->cpu.reg16(3).set(0x5678);
    this->dram->write8(0, 0x19);
    this->dram->write8(1, 0x23);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg16(3).get(), 0x5678 - 0x1234);
}

TEST_F(CPUTestFix, sub_register_direct_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(2).set(0x12345678);
    this->h8->cpu.reg32(3).set(0x9abcdef0);
    this->dram->write8(0, 0x1a);
    this->dram->write8(1, 0xa3);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg32(3).get(), 0x9abcdef0 - 0x12345678);
}

TEST_F(CPUTestFix, sub_immediate_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x9abcdef0);
    this->dram->write8(0, 0x7a);
    this->dram->write8(1, 0x31);
    this->dram->write8(2, 0x12);
    this->dram->write8(3, 0x34);
    this->dram->write8(4, 0x56);
    this->dram->write8(5, 0x78);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x9abcdef0 - 0x12345678);
}
