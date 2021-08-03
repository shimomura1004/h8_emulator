#include <gtest/gtest.h>
#include "cpu_test_fixture.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, or_immediate_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(0).set(0x0f);
    // or.b #0xf0:8,r0l
    this->dram->write8(0, 0xc0);
    this->dram->write8(1, 0xf0);

    // 正しく命令が実行され、8ビットレジスタ R0L に即値 0xff がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(0).get();
    EXPECT_EQ(val, 0xff);
}

TEST_F(CPUTestFix, or_immediate_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x0f0f);
    // or.w #0xf0f0:16,e1
    this->dram->write8(0, 0x79);
    this->dram->write8(1, 0x41);
    this->dram->write16(2, 0xf0f0);

    // 16ビットレジスタ e1 に即値 0xffff がコピーされる
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(1).get();
    EXPECT_EQ(val, 0xffff);
}
