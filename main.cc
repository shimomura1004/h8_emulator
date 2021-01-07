#include "h8300h.h"
#include <stdio.h>

int main (int argc, char* argv[])
{
    H8300H h8;
    h8.init();

    uint32_t start_addr = h8.load_elf("./osbook_03/01/bootload/kzload");
    printf("Start execution from 0x%x\n", start_addr);

    // 本当はリセット割り込みを発生させて開始するべき
    h8.pc = start_addr;

    h8.run(argc != 1);

    // h8.print_registers();

    return 0;
}
