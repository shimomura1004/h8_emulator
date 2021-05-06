#include <stdio.h>
#include "h8board.h"
#include "runner.h"

#include "cpu/h8300h_cpu.h"
#include "dram/h8300h_dram.h"
#include "sci/h8300h_sci.h"
#include "timer/h8300h_timer8.h"
#include "net/rtl8019as.h"
#include "interrupt/general_interrupt_controller.h"
#include "h8300h_mcu.h"

int main (int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s (ELF file) (option)\n", argv[0]);
        exit(1);
    }

    // todo: H8300H で必要なオブジェクトは H8300H クラス内で初期化するほうがいい
    // stdio を使うときは screen コマンドを使うこと
    H8300H_CPU cpu;
    std::condition_variable& interrupt_cv = cpu.get_interrupt_cv();
    H8300H_DRAM dram;
    // todo: H8300H の固有のもの、H8300H クラス内で用意するのであれば配列にする必要もない
    ISCI *sci[3] = {
        new H8300H_SCI(0, interrupt_cv),
        new H8300H_SCI(1, interrupt_cv, false),
        new H8300H_SCI(2, interrupt_cv)
    };
    H8300H_Timer8 timer8(interrupt_cv);
    IOPort ioport;
    RTL8019AS rtl8019as(interrupt_cv);
    GeneralInterruptController interrupt_controller(sci, &timer8, &rtl8019as);
    H8300H_MCU mcu(dram, sci, timer8, ioport, rtl8019as);
    H8Board h8(cpu, mcu, interrupt_controller);
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
