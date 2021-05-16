#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "instructions/mov_register_indirect_displacement_prepost.cc"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_register_indirect_with_displacement_prepost_b_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(7).set(DummyMCU::area2_start + 0x100);
    this->mcu->write8(DummyMCU::area2_start + 0x100, 0x56);

    // mov.b @ER7+,r2l
    this->dram->write8(0, 0x6c);
    this->dram->write8(1, 0x72);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(2).get();
    EXPECT_EQ(val, 0x56);

    // スタックポインタが1バイト分、解放される
    int sp = this->cpu->sp().get();
    EXPECT_EQ(sp, DummyMCU::area2_start + 0x100 + 1);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement_prepost_b_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(7).set(DummyMCU::area2_start + 0x100);
    this->cpu->reg8(2).set(0x78);

    // mov.b r2l,@-er7
    this->dram->write8(0, 0x6c);
    this->dram->write8(1, 0xf2);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read8(DummyMCU::area2_start + 0x100 - 1);
    EXPECT_EQ(val, 0x78);

    // スタックポインタが1バイト分、消費される
    int sp = this->cpu->sp().get();
    EXPECT_EQ(sp, DummyMCU::area2_start + 0x100 - 1);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement_prepost_l_1)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(7).set(DummyMCU::area2_start + 0x100);
    this->mcu->write32(DummyMCU::area2_start + 0x100, 0x12345678);

    // mov.l @ER7+,er2
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x6d);
    this->dram->write8(3, 0x72);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(2).get();
    EXPECT_EQ(val, 0x12345678);

    // スタックポインタが4バイト分、解放される
    int sp = this->cpu->sp().get();
    EXPECT_EQ(sp, DummyMCU::area2_start + 0x100 + 4);
}

TEST_F(CPUTestFix, mov_register_indirect_with_displacement_prepost_l_2)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->cpu->reg32(7).set(DummyMCU::area2_start + 0x100);
    this->cpu->reg32(2).set(0x567890ab);

    // mov.l er2,@-er7
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x6d);
    this->dram->write8(3, 0xf2);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->mcu->read32(DummyMCU::area2_start + 0x100 - 4);
    EXPECT_EQ(val, 0x567890ab);

    // スタックポインタが4バイト分、消費される
    int sp = this->cpu->sp().get();
    EXPECT_EQ(sp, DummyMCU::area2_start + 0x100 - 4);
}

