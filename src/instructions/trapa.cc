#include "trapa.h"

static interrupt_t interrupt_table[4] = {
    interrupt_t::TRAP0,
    interrupt_t::TRAP1,
    interrupt_t::TRAP2,
    interrupt_t::TRAP3,
};

namespace h8instructions {
namespace trapa {

void trapa_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "trapa";
    instruction.op1.set_immediate8((b1 & 0x30) >> 4);
    instruction.op2.set_not_used();

    instruction.parser = trapa_parse;
    instruction.runner = trapa_run;
}

int trapa_run(H8Board* h8, Instruction& instruction)
{
    uint8_t imm = instruction.op1.get_immediate8();

    // 割り込みを発生させる
    h8->interrupt_controller.set(interrupt_table[imm]);

    // CCR.I ビットは割り込み処理部でセットされる

    h8->cpu.pc() += 2;

    return 0;
}

}
}
