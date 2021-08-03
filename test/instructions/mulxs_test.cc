#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mulxs_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg16(1).set(0x1234);
    this->h8->cpu.reg32(2).set(0x5678);
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0xc0);
    this->dram->write8(2, 0x52);
    this->dram->write8(3, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.reg32(2).get(), 0x1234 * 0x5678);
}
