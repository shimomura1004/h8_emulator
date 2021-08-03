#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, not_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0x12);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    uint8_t expected = 0x12;
    expected = ~expected;
    EXPECT_EQ(this->h8->cpu.reg8(1).get(), expected);
}

TEST_F(CPUTestFix, not_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x1234);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x11);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    uint16_t expected = 0x1234;
    expected = ~expected;
    EXPECT_EQ(this->h8->cpu.reg16(1).get(), expected);
}

TEST_F(CPUTestFix, not_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x12345678);
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x31);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    uint32_t expected = 0x12345678;
    expected = ~expected;
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), expected);
}
