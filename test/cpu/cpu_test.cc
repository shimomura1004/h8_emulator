#include <gtest/gtest.h>
#include "cpu/cpu.h"

/*
H8/300H マイコンのレジスタ構成

- ビッグエンディアン

        15              0 7      0 7      0
        +----------------+--------+--------+
ER0     |       E0       |  R0H   |   R0L  |
        +----------------+--------+--------+
ER1     |       E1       |  R1H   |   R1L  |
        +----------------+--------+--------+
...             ...         ...       ...
        +----------------+--------+--------+
ER7(SP) |       E7       |  R7H   |   R7L  |
        +----------------+--------+--------+
*/

// ER レジスタに値を書き込むと E/R/RH/RL に値が反映される
TEST(H8300H_CPU, reg32)
{
     H8300H_CPU cpu;

     Register32& er0 = cpu.reg32(0);
     Register16& e0  = cpu.reg16(8);
     Register16& r0  = cpu.reg16(0);
     Register8&  r0h = cpu.reg8(0);
     Register8&  r0l = cpu.reg8(8);

     er0.set(0x12345678);

     EXPECT_EQ(e0.get(), 0x1234);
     EXPECT_EQ(r0.get(), 0x5678);

     EXPECT_EQ(r0h.get(), 0x56);
     EXPECT_EQ(r0l.get(), 0x78);
}

// R レジスタに値を書き込むと RH/RL に値が反映される
TEST(H8300H_CPU, reg16)
{
     H8300H_CPU cpu;

     Register16& r0  = cpu.reg16(0);
     Register8&  r0h = cpu.reg8(0);
     Register8&  r0l = cpu.reg8(8);

     r0.set(0x1234);

     EXPECT_EQ(r0h.get(), 0x12);
     EXPECT_EQ(r0l.get(), 0x34);
}

// ER7 と SP は同じレジスタ
TEST(H8300H_CPU, sp)
{
     H8300H_CPU cpu;

     Register32& er7 = cpu.reg32(7);
     Register32& sp  = cpu.sp();

     er7.set(0x00100000);
     EXPECT_EQ(sp.get(), 0x00100000);

     sp.set(0x00200000);
     EXPECT_EQ(er7.get(), 0x00200000);
}
