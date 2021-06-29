#include "andc.h"

namespace h8instructions {
namespace andc {

void andc_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "andc";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_ccr();

    instruction.parser = andc_parse;
    instruction.runner = andc_run;
}

int andc_run(H8Board* h8, Instruction& instruction)
{
    uint8_t imm = instruction.op1.get_immediate8();
    h8->cpu.ccr().set(h8->cpu.ccr().raw() & imm);
    h8->cpu.pc() += 2;

    return 0;
}

}
}
