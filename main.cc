#include "h8300h.h"
#include <stdio.h>

int main (int argc, char* argv[])
{
    H8300H h8;
    // uint32_t start_addr = h8.load_elf("/Users/shimo/src/h8_emulator/kozos/06/os/kozos");
    uint32_t start_addr = h8.load_elf("/Users/shimo/src/h8_emulator/kozos/01/kzload");
    printf("Start from 0x%x\n", start_addr);

    // h8.memory.dump("./hoge");

    // 本当はリセット割り込みを発生させて開始するべき
    h8.pc = start_addr;

    // h8.step();
    h8.run();

    h8.print_registers();

    return 0;
}

void test()
{
    Register32 reg;

    reg.set_er(0x12345678);
    printf("0x%08x\n", reg.get_er());

    printf("0x%08x\n", reg.get_e());
    reg.set_e(0xabcd);
    printf("0x%08x\n", reg.get_e());

    printf("0x%08x\n", reg.get_r());
    reg.set_r(0x9876);
    printf("0x%08x\n", reg.get_r());

    printf("0x%08x 0x%08x\n", reg.get_rh(), reg.get_rl());
    reg.set_rh(0x22);
    reg.set_rl(0x44);
    printf("0x%08x 0x%08x\n", reg.get_rh(), reg.get_rl());

    printf("0x%08x\n", reg.get_er());
}