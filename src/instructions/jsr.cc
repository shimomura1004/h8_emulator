#include "jsr.h"

namespace h8instructions {
namespace jsr {

void register_indirect_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "jsr";
    instruction.op1.set_register_indirect((b1 & 0x70) >> 4);
    instruction.op2.set_not_used();

    instruction.parser = register_indirect_parse;
    instruction.runner = jsr_register_indirect_run;
}

int jsr_register_indirect_run(H8Board* h8, Instruction& instruction)
{
    const Register32& reg = h8->cpu.reg32(instruction.op1.get_register_indirect());

    uint32_t address = reg.get();

    h8->push_to_stack_l(h8->cpu.pc() + 2);
    h8->cpu.pc() = address;

    return 0;
}

void absolute_address_parse(H8Board* h8, Instruction& instruction)
{
    uint32_t absolute = h8instructions::parse_immediate<uint32_t>(h8, 1, 3, false);

    instruction.name = "jsr";
    instruction.op1.set_absolute_address24(absolute);
    instruction.op2.set_not_used();

    instruction.parser = absolute_address_parse;
    instruction.runner = jsr_absolute_address_run;
}

int jsr_absolute_address_run(H8Board* h8, Instruction& instruction)
{
    h8->push_to_stack_l(h8->cpu.pc() + 4);
    h8->cpu.pc() = instruction.op1.get_absolute_address24();

    return 0;
}

}
}
