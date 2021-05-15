#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "instructions/mov_register_indirect.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_register_indirect_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);
    this->mcu->write8(DummyMCU::area2_start + 0x1000, 0x34);

    // mov.b @er1,r2l
    this->dram->write8(0, 0x68);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(2).get();
    EXPECT_EQ(val, 0x34);
}

TEST_F(CPUTestFix, mov_register_indirect_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg8(2).set(0x34);
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);

    // mov.b r2l,@er1
    this->dram->write8(0, 0x68);
    this->dram->write8(1, 0x92);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read8(DummyMCU::area2_start + 0x1000);
    EXPECT_EQ(val, 0x34);
}

TEST_F(CPUTestFix, mov_register_indirect_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);
    this->mcu->write16(DummyMCU::area2_start + 0x1000, 0x5678);

    // mov.w @er1,r2
    this->dram->write8(0, 0x69);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(2).get();
    EXPECT_EQ(val, 0x5678);
}

TEST_F(CPUTestFix, mov_register_indirect_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg16(2).set(0x5678);
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);

    // mov.w r2,@er1
    this->dram->write8(0, 0x69);
    this->dram->write8(1, 0x92);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read16(DummyMCU::area2_start + 0x1000);
    EXPECT_EQ(val, 0x5678);
}

TEST_F(CPUTestFix, mov_register_indirect_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);
    this->mcu->write32(DummyMCU::area2_start + 0x1000, 0x567890ab);

    // mov.l @er1,er2
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x69);
    this->dram->write8(3, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(2).get();
    EXPECT_EQ(val, 0x567890ab);
}

TEST_F(CPUTestFix, mov_register_indirect_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->h8->cpu.reg32(2).set(0x567890ab);
    this->h8->cpu.reg32(1).set(DummyMCU::area2_start + 0x1000);

    // mov.l er2,@er1
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x69);
    this->dram->write8(3, 0x92);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read32(DummyMCU::area2_start + 0x1000);
    EXPECT_EQ(val, 0x567890ab);
}
