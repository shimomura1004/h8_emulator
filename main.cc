#include <stdio.h>
#include "h8300h.h"
#include "runner.h"

int main (int argc, char* argv[])
{
    H8300H h8;
    h8.init();

    uint32_t start_addr = h8.load_elf("./osbook_03/01/bootload/kzload");
    // uint32_t start_addr = h8.load_elf("./osbook_03/02/bootload/kzload");
    printf("Start execution from 0x%x\n", start_addr);

    // 本当はリセット割り込みを発生させて開始するべき
    h8.pc = start_addr;

    Runner runner(h8);
    runner.run(argc != 1);

    return 0;
}
