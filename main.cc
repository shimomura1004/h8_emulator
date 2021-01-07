#include "h8300h.h"
#include <stdio.h>

int main (int argc, char* argv[])
{
    H8300H h8;
    h8.init();

    uint32_t start_addr = h8.load_elf("./osbook_03/01/bootload/kzload");
    printf("Start execution from 0x%x\n", start_addr);

    // todo: シリアルが割り当てられたアドレスへの書き込みを認識する必要あり
    // 外部デバイスの動作をエミュレートする必要がある
    // たぶんデバイスごとにスレッドにしてメモリを共有するのが楽
    
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