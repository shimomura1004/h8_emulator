#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/cmp.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, cmp_immediate_b_ccr_z)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0x0f);
    // cmp.b #0x0f:8,r0l
    this->dram->write8(0, 0xA0);
    this->dram->write8(1, 0x0f);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0x0f);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_immediate_b_ccr_h)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0b01000000);
    this->dram->write8(0, 0xA0);
    this->dram->write8(1, 0b00001000);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0b01000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_immediate_b_ccr_n_c)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0x10);
    this->dram->write8(0, 0xA0);
    this->dram->write8(1, 0x20);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0x10);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, cmp_immediate_b_ccr_n_v)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0b00000000);
    this->dram->write8(0, 0xA0);
    this->dram->write8(1, 0b10000000);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0b00000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}


TEST_F(CPUTestFix, cmp_immediate_w_ccr_z)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0x0f0f);
    // cmp.w #0x0f0f:8,r0
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x20);
    this->dram->write8(2, 0x0f);
    this->dram->write8(3, 0x0f);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0x0f0f);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_immediate_w_ccr_h)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0b0100000000000000);
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x20);
    // 14ビット目にボローを発生させる
    this->dram->write8(2, 0b00001000);
    this->dram->write8(3, 0b00000000);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0b0100000000000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_immediate_w_ccr_n_c)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0x1000);
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x20);
    this->dram->write8(2, 0x20);
    this->dram->write8(3, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0x1000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, cmp_immediate_w_ccr_n_v)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0x00);
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x20);
    this->dram->write8(2, 0x80);
    this->dram->write8(3, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0x00);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}
