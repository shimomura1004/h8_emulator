#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/and.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, and_immediate_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b10101010);
    // and.b #0b10101010,r1l
    this->dram->write8(0, 0xe1);
    this->dram->write8(1, 0xf0);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b10100000);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, and_immediate_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0xffff);
    // and.w #0xffff,r1
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x61);
    this->dram->write8(2, 0x0f);
    this->dram->write8(3, 0x0f);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg16(1).get(), 0x0f0f);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

TEST_F(CPUTestFix, and_immediate_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg32(1).set(0xf0f0f0f0);
    // and.l #0xffffffff,er1
    this->dram->write8(0, 0x7a);
    this->dram->write8(1, 0x61);
    this->dram->write8(2, 0xff);
    this->dram->write8(3, 0xff);
    this->dram->write8(4, 0xff);
    this->dram->write8(5, 0xff);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0xf0f0f0f0);
    EXPECT_EQ(this->h8->cpu.ccr().n(), true);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}

// TEST_F(CPUTestFix, and_register_direct_b)
// {
//     this->h8->cpu.pc() = DummyMCU::area2_start;

//     this->h8->cpu.reg8(0).set(0b11111111);
//     this->h8->cpu.reg8(1).set(0b10101010);
//     // and.b #r0l,r1l
//     this->dram->write8(0, 0x15);
//     this->dram->write8(1, 0x01);

//     int ret = this->h8->execute_next_instruction();
//     EXPECT_EQ(ret, 0);

//     EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b01010101);
//     EXPECT_EQ(this->h8->cpu.ccr().n(), false);
//     EXPECT_EQ(this->h8->cpu.ccr().z(), false);
//     EXPECT_EQ(this->h8->cpu.ccr().v(), false);
// }

// TEST_F(CPUTestFix, and_register_direct_l)
// {
//     this->h8->cpu.pc() = DummyMCU::area2_start;

//     this->h8->cpu.reg32(0).set(0xffffffff);
//     this->h8->cpu.reg32(1).set(0xaaaaaaaa); // 0b10101010 = 0xaa
//     // and.l #er0,er1
//     this->dram->write8(0, 0x01);
//     this->dram->write8(1, 0xf0);
//     this->dram->write8(2, 0x65);
//     this->dram->write8(3, 0x01);

//     int ret = this->h8->execute_next_instruction();
//     EXPECT_EQ(ret, 0);

//     EXPECT_EQ(this->h8->cpu.reg32(1).get(), 0x55555555); // 0b01010101 = 0x55
//     EXPECT_EQ(this->h8->cpu.ccr().n(), false);
//     EXPECT_EQ(this->h8->cpu.ccr().z(), false);
//     EXPECT_EQ(this->h8->cpu.ccr().v(), false);
// }
