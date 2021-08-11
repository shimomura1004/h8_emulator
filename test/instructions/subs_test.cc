#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

TEST_F(CPUTestFix, subs_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(12);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 12 - 1);
}

TEST_F(CPUTestFix, subs_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(12);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x81);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 12 - 2);
}

TEST_F(CPUTestFix, subs_3)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(12);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 12 - 4);
}

TEST_F(CPUTestFix, subs_4)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(4);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 4 - 4);
}

TEST_F(CPUTestFix, subs_5)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 0 - 4);
}

TEST_F(CPUTestFix, subs_6)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(-10);
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), -10 - 4);
}
