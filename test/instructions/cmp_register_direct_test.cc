#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, cmp_register_direct_b_ccr_z)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0x0f);
    this->h8->cpu.reg8(1).set(0x0f);
    // cmp.b r0l,r1l
    this->dram->write8(0, 0x1c);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0x0f);
    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0x0f);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_b_ccr_h)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0b00001000);
    this->h8->cpu.reg8(1).set(0b01000000);
    // cmp.b r0l,r1l
    this->dram->write8(0, 0x1c);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0b00001000);
    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b01000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_b_ccr_n_c)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0x20);
    this->h8->cpu.reg8(1).set(0x10);
    // cmp.b r0l,r1l
    this->dram->write8(0, 0x1c);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0x20);
    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0x10);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, cmp_register_direct_b_ccr_n_v)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0b10000000);
    this->h8->cpu.reg8(1).set(0b00000000);
    // cmp.b r0l,r1l
    this->dram->write8(0, 0x1c);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg8(0).get(), 0b10000000);
    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b00000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}


TEST_F(CPUTestFix, cmp_register_direct_w_ccr_z)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0x0f0f);
    this->h8->cpu.reg16(1).set(0x0f0f);
    // cmp.w r0,r1
    this->dram->write8(0, 0x1d);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0x0f0f);
    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x0f0f);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_w_ccr_h)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0b0000100000000000);
    this->h8->cpu.reg16(1).set(0b0100000000000000);
    // cmp.w r0,r1
    this->dram->write8(0, 0x1d);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0b0000100000000000);
    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0b0100000000000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_w_ccr_n_c)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0x2000);
    this->h8->cpu.reg16(1).set(0x1000);
    // cmp.w r0,r1
    this->dram->write8(0, 0x1d);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0x2000);
    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x1000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, cmp_register_direct_w_ccr_n_v)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(0).set(0b1000000000000000);
    this->h8->cpu.reg16(1).set(0b0000000000000000);
    // cmp.w r0,r1
    this->dram->write8(0, 0x1d);
    this->dram->write8(1, 0x01);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg16(0).get(), 0b1000000000000000);
    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0b0000000000000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}


TEST_F(CPUTestFix, cmp_register_direct_l_ccr_z)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(0).set(0x0f0f0f0f);
    this->h8->cpu.reg32(1).set(0x0f0f0f0f);
    this->dram->write8(0, 0x1f);
    this->dram->write8(1, 0x81);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg32(0).get(), 0x0f0f0f0f);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x0f0f0f0f);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), true);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_l_ccr_h)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(0).set(0b00001000000000000000000000000000);
    this->h8->cpu.reg32(1).set(0b01000000000000000000000000000000);
    this->dram->write8(0, 0x1f);
    this->dram->write8(1, 0x81);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg32(0).get(), 0b00001000000000000000000000000000);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0b01000000000000000000000000000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), true);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), false);
}

TEST_F(CPUTestFix, cmp_register_direct_l_ccr_n_c)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(0).set(0x20000000);
    this->h8->cpu.reg32(1).set(0x10000000);
    this->dram->write8(0, 0x1f);
    this->dram->write8(1, 0x81);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg32(0).get(), 0x20000000);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x10000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}

TEST_F(CPUTestFix, cmp_register_direct_l_ccr_n_v)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(0).set(0b10000000000000000000000000000000);
    this->h8->cpu.reg32(1).set(0b00000000000000000000000000000000);
    this->dram->write8(0, 0x1f);
    this->dram->write8(1, 0x81);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // レジスタの値は変化しない
    EXPECT_EQ(this->h8->cpu.reg32(0).get(), 0b10000000000000000000000000000000);
    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0b00000000000000000000000000000000);

    EXPECT_EQ(this->h8->cpu.ccr().h(), false);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), true);
    EXPECT_EQ(this->h8->cpu.ccr().c(), true);
}
