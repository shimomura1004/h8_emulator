#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "instructions/mov_immediate.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_immediate_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // mov.b #0x12:8,r0l
    this->dram->write8(0, 0xf0);
    this->dram->write8(1, 0x12);

    // 正しく命令が実行され、8ビットレジスタ R0L に即値 0x12 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(0).get();
    EXPECT_EQ(val, 0x12);
}

TEST_F(CPUTestFix, mov_immediate_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // mov.w #0x3456:16,e1
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x01);
    this->dram->write16(2, 0x3456);

    // 16ビットレジスタ e1 に即値 0x3456 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(1).get();
    EXPECT_EQ(val, 0x3456);
}

TEST_F(CPUTestFix, mov_immediate_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    // mov.l #0x12345678:32,er2
    this->dram->write8(0, 0x7A);
    this->dram->write8(1, 0x02);
    this->dram->write32(2, 0x12345678);

    // 32ビットレジスタ er2 に即値 0x12345678 がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(2).get();
    EXPECT_EQ(val, 0x12345678);
}
