#include "trapa.h"

static interrupt_t interrupt_table[4] = {
    interrupt_t::TRAP0,
    interrupt_t::TRAP1,
    interrupt_t::TRAP2,
    interrupt_t::TRAP3,
};

int h8instructions::trapa::trapa(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm = (b1 & 0x30) >> 4;

    // 割り込みを発生させる
    h8->interrupt_controller.set(interrupt_table[imm]);

    // CCR.I ビットは割り込み処理部でセットされる

    h8->cpu.pc() += 2;

    return 0;
}
