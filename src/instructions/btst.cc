#include "btst.h"

namespace h8instructions {
namespace btst {

void register_direct_imm_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "btst";
    instruction.op1.set_immediate8((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = register_direct_imm_parse;
    instruction.runner = register_direct_imm_run;
}

int register_direct_imm_run(H8Board* h8, Instruction& instruction)
{
    const Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t mask = (1 << instruction.op1.get_immediate8());
    uint8_t value = dst.get();

    (value & mask) ? h8->cpu.ccr().clear_z() : h8->cpu.ccr().set_z();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
