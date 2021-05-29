#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/bcc.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, bra8)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // bra #0x10:8
    this->dram->write8(0, 0x40);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bra16)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // bra #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4 + 0x1001);
}


TEST_F(CPUTestFix, bhi8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().clear_z();

    // bhi #0x10:8
    this->dram->write8(0, 0x42);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bhi8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_c();
    this->h8->cpu.ccr().clear_z();

    // bhi #0x10:8
    this->dram->write8(0, 0x42);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}

TEST_F(CPUTestFix, bhi16_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().clear_z();

    // bhi #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x20);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4 + 0x1001);
}

TEST_F(CPUTestFix, bhi16_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().set_z();

    // bhi #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x20);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4);
}


TEST_F(CPUTestFix, bls8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_c();
    this->h8->cpu.ccr().clear_z();

    // bls #0x10:8
    this->dram->write8(0, 0x43);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bls8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().clear_z();

    // bls #0x10:8
    this->dram->write8(0, 0x43);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}

TEST_F(CPUTestFix, bls16_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().set_z();

    // bls #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x30);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4 + 0x1001);
}

TEST_F(CPUTestFix, bls16_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();
    this->h8->cpu.ccr().clear_z();

    // bls #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x30);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4);
}


TEST_F(CPUTestFix, bcc8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();

    // bcs #0x10:8
    this->dram->write8(0, 0x44);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bcc8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_c();

    // bcs #0x10:8
    this->dram->write8(0, 0x44);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}


TEST_F(CPUTestFix, bcs8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_c();

    // bcs #0x10:8
    this->dram->write8(0, 0x45);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bcs8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_c();

    // bcs #0x10:8
    this->dram->write8(0, 0x45);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}


TEST_F(CPUTestFix, bne8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_z();

    // bne #0x10:8
    this->dram->write8(0, 0x46);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, bne8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_z();

    // bne #0x10:8
    this->dram->write8(0, 0x46);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}

TEST_F(CPUTestFix, bne16_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_z();

    // bne #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x60);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4 + 0x1001);
}

TEST_F(CPUTestFix, bne16_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_z();

    // bne #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x60);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4);
}


TEST_F(CPUTestFix, beq8_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_z();

    // beq #0x10:8
    this->dram->write8(0, 0x47);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2 + 0x10);
}

TEST_F(CPUTestFix, beq8_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_z();

    // bq #0x10:8
    this->dram->write8(0, 0x47);
    this->dram->write8(1, 0x10);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 2);
}

TEST_F(CPUTestFix, beq16_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().set_z();

    // beq #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x70);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4 + 0x1001);
}

TEST_F(CPUTestFix, beq16_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.ccr().clear_z();

    // beq #0x1001:16
    this->dram->write8(0, 0x58);
    this->dram->write8(1, 0x70);
    this->dram->write8(2, 0x10);
    this->dram->write8(3, 0x01);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.pc(), DummyMCU::area2_start + 4);
}
