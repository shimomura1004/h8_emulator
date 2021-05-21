#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "instructions/mov_register_indirect_displacement16.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->mcu->write8(DummyMCU::ram_start + 0x1234, 0x56);

    // mov.b @(0x1234:16,er1),r2
    this->dram->write8(0, 0x6e);
    this->dram->write8(1, 0x12);
    this->dram->write8(2, 0x12);
    this->dram->write8(3, 0x34);

    // 正しく命令が実行され、8ビットレジスタ R2L に、メモリから 0x56 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(2).get();
    EXPECT_EQ(val, 0x56);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->cpu->reg8(2).set(0x78);

    // mov.b r2,@(0x1234:16,er1)
    this->dram->write8(0, 0x6e);
    this->dram->write8(1, 0x92);
    this->dram->write8(2, 0x12);
    this->dram->write8(3, 0x34);

    // 正しく命令が実行され、メモリに、レジスタ R2L から 0x78 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read8(DummyMCU::ram_start + 0x1234);
    EXPECT_EQ(val, 0x78);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_w_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->mcu->write16(DummyMCU::ram_start + 0x1234, 0x5678);

    // mov.w @(0x1234:16,er1),r2
    this->dram->write8(0, 0x6f);
    this->dram->write8(1, 0x12);
    this->dram->write8(2, 0x12);
    this->dram->write8(3, 0x34);

    // 正しく命令が実行され、16ビットレジスタ R2 に、メモリから 0x5678 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(2).get();
    EXPECT_EQ(val, 0x5678);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_w_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->cpu->reg16(2).set(0x7890);

    // mov.w r2,@(0x1234:16,er1)
    this->dram->write8(0, 0x6f);
    this->dram->write8(1, 0x92);
    this->dram->write8(2, 0x12);
    this->dram->write8(3, 0x34);

    // 正しく命令が実行され、メモリに、レジスタ R2 から 0x7890 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read16(DummyMCU::ram_start + 0x1234);
    EXPECT_EQ(val, 0x7890);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->mcu->write32(DummyMCU::ram_start + 0x1234, 0x567890ab);

    // mov.l @(0x1234:16,er1),er2
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x6f);
    this->dram->write8(3, 0x12);
    this->dram->write8(4, 0x12);
    this->dram->write8(5, 0x34);

    // 正しく命令が実行され、32ビットレジスタ ER2 に、メモリから 0x567890ab がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(2).get();
    EXPECT_EQ(val, 0x567890ab);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement16_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(1).set(DummyMCU::ram_start);
    this->cpu->reg32(2).set(0x7890abcd);

    // mov.l er2,@(0x1234:16,er1)
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x6f);
    this->dram->write8(3, 0x92);
    this->dram->write8(4, 0x12);
    this->dram->write8(5, 0x34);

    // 正しく命令が実行され、メモリに、レジスタ R2 から 0x7890 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read32(DummyMCU::ram_start + 0x1234);
    EXPECT_EQ(val, 0x7890abcd);
}