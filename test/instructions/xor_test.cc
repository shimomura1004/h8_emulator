#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/xor.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, xor_immediate_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b10101010);
    // xor.b #0b10101010,r1l
    this->dram->write8(0, 0xd1);
    this->dram->write8(1, 0xff);

    // レジスタから即値を引いた結果を比較
    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg8(1).get(), 0b01010101);
    EXPECT_EQ(this->h8->cpu.ccr().n(), false);
    EXPECT_EQ(this->h8->cpu.ccr().z(), false);
    EXPECT_EQ(this->h8->cpu.ccr().v(), false);
}
