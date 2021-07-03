#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "cpu/ccr.h"
#include "instructions/btst.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, btst_register_direct_immediate_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b00000010);
    this->dram->write8(0, 0x73);
    this->dram->write8(1, 0x11);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    // 指定位置のビットが立っていると、z がクリアされる
    EXPECT_EQ(this->h8->cpu.ccr().z(), 0);
}

TEST_F(CPUTestFix, btst_register_direct_immediate_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;

    this->h8->cpu.reg8(1).set(0b00000010);
    this->dram->write8(0, 0x73);
    this->dram->write8(1, 0x12);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(this->h8->cpu.ccr().z(), 1);
}

