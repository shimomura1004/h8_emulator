#include "jmp.h"

namespace h8instructions {
namespace jmp {

void register_indirect_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "jmp";
    instruction.op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction.op2.set_not_used();

    instruction.parser = register_indirect_parse;
    instruction.runner = jmp_register_indirect_run;
}

int jmp_register_indirect_run(H8Board* h8, Instruction& instruction)
{
    const Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());
    uint32_t address = reg.get();

    h8->cpu.pc() = address;

    return 0;
}

}
}
