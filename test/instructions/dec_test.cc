#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

TEST_F(CPUTestFix, dec_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(12);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1a);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int8_t)this->h8->cpu.reg8(1).get(), 12 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1a);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int8_t)this->h8->cpu.reg8(1).get(), 1 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_b_3)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(-10);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1a);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int8_t)this->h8->cpu.reg8(1).get(), -10 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_b_4)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0x80);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1a);
    this->dram->write8(1, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int8_t)this->h8->cpu.reg8(1).get(), (int8_t)0x7f);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}


TEST_F(CPUTestFix, dec_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(1234);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x51);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), 1234 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x51);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), 1 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_w_3)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x51);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), 0 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_w_4)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x8000);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x51);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), (int16_t)0x7fff);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}

TEST_F(CPUTestFix, dec_w_5)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x8000);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0xd1);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), (int16_t)0x7ffe);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}

TEST_F(CPUTestFix, dec_w_6)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x8001);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0xd1);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), (int16_t)0x7fff);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}


TEST_F(CPUTestFix, dec_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(123456);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x71);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 123456 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(1);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x71);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 1 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_l_3)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x71);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), 0 - 1);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, dec_l_4)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x80000000);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0x71);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), (int32_t)0x7fffffff);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}

TEST_F(CPUTestFix, dec_l_5)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x80000000);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0xf1);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), (int32_t)0x7ffffffe);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}

TEST_F(CPUTestFix, dec_l_6)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0x80000001);
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->dram->write8(0, 0x1b);
    this->dram->write8(1, 0xf1);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int32_t)this->h8->cpu.reg32(1).get(), (int32_t)0x7fffffff);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
}
