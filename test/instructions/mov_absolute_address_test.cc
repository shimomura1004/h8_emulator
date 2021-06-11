#include <gtest/gtest.h>
#include "cpu_test_fixture.h"
#include "instructions/mov_absolute_address.h"

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果

TEST_F(CPUTestFix, mov_absolute_address_24_b)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->mcu->write8(DummyMCU::area2_start + 0x1000, 0x56);

    // RAM の開始アドレス(ram_start)は 0x400000
    // mov.b @0x401000:24,r1l
    this->dram->write8(0, 0x6a);
    this->dram->write8(1, 0x21);
    this->dram->write8(2, 0x00);
    this->dram->write8(3, 0x40);
    this->dram->write8(4, 0x10);
    this->dram->write8(5, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(1).get();
    EXPECT_EQ(val, 0x56);
}

TEST_F(CPUTestFix, mov_absolute_address_24_b_minus)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->mcu->write8(0x800000, 0x56);

    // mov.b @0x800000:24,r1l
    this->dram->write8(0, 0x6a);
    this->dram->write8(1, 0x21);
    this->dram->write8(2, 0x00);
    this->dram->write8(3, 0x80);
    this->dram->write8(4, 0x00);
    this->dram->write8(5, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg8(1).get();
    EXPECT_EQ(val, 0x56);
}


TEST_F(CPUTestFix, mov_absolute_address_24_w)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->mcu->write16(DummyMCU::area2_start + 0x1000, 0x5678);

    // RAM の開始アドレス(ram_start)は 0x400000
    // mov.w @0x401000:24,r1
    this->dram->write8(0, 0x6b);
    this->dram->write8(1, 0x21);
    this->dram->write8(2, 0x00);
    this->dram->write8(3, 0x40);
    this->dram->write8(4, 0x10);
    this->dram->write8(5, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg16(1).get();
    EXPECT_EQ(val, 0x5678);
}

TEST_F(CPUTestFix, mov_absolute_address_24_l)
{
    this->h8->cpu.pc() = DummyMCU::area2_start;
    this->mcu->write32(DummyMCU::area2_start + 0x1000, 0x567890ab);

    // RAM の開始アドレス(ram_start)は 0x400000
    // mov.l @0x401000:24,er1
    this->dram->write8(0, 0x01);
    this->dram->write8(1, 0x00);
    this->dram->write8(2, 0x6b);
    this->dram->write8(3, 0x21);
    this->dram->write8(4, 0x00);
    this->dram->write8(5, 0x40);
    this->dram->write8(6, 0x10);
    this->dram->write8(7, 0x00);

    int ret = this->h8->execute_next_instruction();
    EXPECT_EQ(ret, 0);
    int val = this->cpu->reg32(1).get();
    EXPECT_EQ(val, 0x567890ab);
}
