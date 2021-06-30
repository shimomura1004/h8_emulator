#include "orc.h"

namespace h8instructions {
namespace orc {

void orc_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "orc";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_ccr();

    instruction.parser = orc_parse;
    instruction.runner = orc_run;
}

int orc_run(H8Board* h8, Instruction& instruction)
{
    uint8_t imm = instruction.op1.get_immediate8();
    h8->cpu.ccr().set(h8->cpu.ccr().raw() | imm);
    h8->cpu.pc() += 2;

    return 0;
}

}
}
