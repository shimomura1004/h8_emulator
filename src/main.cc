#include <stdio.h>
#include "h8300h.h"
#include "runner.h"

#include "cpu/h8300h_cpu.h"

int main (int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s (ELF file) (option)\n", argv[0]);
        exit(1);
    }

    // stdio を使うときは screen コマンドを使うこと
    H8300H_CPU cpu;
    H8300H h8(cpu);
    h8.init();

    uint32_t start_addr = h8.load_elf(argv[1]);
    if (start_addr == 0) {
        exit(1);
    }
    
    printf("Start execution from 0x%x\n", start_addr);

    // 本当はリセット割り込みを発生させて開始するべき
    h8.cpu.pc() = start_addr;

    Runner runner(h8);
    runner.run(argc > 2);

    return 0;
}
