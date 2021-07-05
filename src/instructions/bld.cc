#include "bld.h"

namespace h8instructions {
namespace bld {

void bld_register_direct_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "bld";
    instruction.op1.set_immediate8((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = bld_register_direct_parse;
    instruction.runner = bld_register_direct_run;
}

int bld_register_direct_run(H8Board* h8, Instruction& instruction)
{
    const Register8& reg = h8->cpu.reg8(instruction.op2.get_register_direct8());

    bool nth_bit = reg.get() & (1 << instruction.op1.get_immediate8());

    if (nth_bit) {
        h8->cpu.ccr().set_c();
    } else {
        h8->cpu.ccr().clear_c();
    }

    h8->cpu.pc() += 2;

    return 0;
}

}
}
