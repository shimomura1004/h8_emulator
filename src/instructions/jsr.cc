#include "jsr.h"

int h8instructions::jsr::jsr_absolute_address(H8Board* h8)
{
    unsigned char abs_[4];
    abs_[3] = 0;
    abs_[2] = h8->fetch_instruction_byte(1);
    abs_[1] = h8->fetch_instruction_byte(2);
    abs_[0] = h8->fetch_instruction_byte(3);
    int32_t abs = *(int32_t*)abs_;

    h8->push_to_stack_l(h8->cpu.pc() + 4);
    h8->cpu.pc() = abs;

    return 0;
}

namespace h8instructions {
namespace jsr {

void jsr_register_indirect_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "jsr";
    instruction.op1.set_register_indirect((b1 & 0x70) >> 4);
    instruction.op2.set_not_used();

    instruction.parser = jsr_register_indirect_parse;
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

void jsr_absolute_address_parse(H8Board* h8, Instruction& instruction)
{

}

int jsr_absolute_address_run(H8Board* h8, Instruction& instruction)
{

}

}
}
