#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// CCR の更新規則
// H = Dm-4 + Rm-4
// N = Rm
// Z = !Rm*!R(m-1)*...*!R0
// V = Dm*Rm
// C = Dm + Rm

TEST_F(CPUTestFix, neg_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(12);
    this->h8->cpu.ccr().clear_h();
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->h8->cpu.ccr().clear_c();
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), -12);
    // H = Dm-4 + Rm-4 = D11 + R11 = true
    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    // N = Rm = R15 = true
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    // Z = !Rm*!R(m-1)*...*!R0 = false
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    // V = Dm*Rm = D15 * R15 = 1 * 0 = 0
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    // C = Dm + Rm = D15 + R15 = true
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, neg_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // 0x8000 のときは negate しない
    this->h8->cpu.reg16(1).set(0x8000);
    this->h8->cpu.ccr().clear_h();
    this->h8->cpu.ccr().clear_n();
    this->h8->cpu.ccr().clear_z();
    this->h8->cpu.ccr().clear_v();
    this->h8->cpu.ccr().clear_c();
    this->dram->write8(0, 0x17);
    this->dram->write8(1, 0x91);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ((int16_t)this->h8->cpu.reg16(1).get(), (int16_t)0x8000);
    // H = Dm-4 + Rm-4 = D11 + R11 = false
    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    // N = Rm = R15 = true
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    // Z = !Rm*!R(m-1)*...*!R0 = false
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    // V = Dm*Rm = D15 * R15 = 1 * 1 = 1
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    // C = Dm + Rm = D15 + R15 = true
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}
