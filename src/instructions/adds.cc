#include "adds.h"

namespace h8instructions {
namespace adds {

void adds_parse(H8Board* h8, Instruction& instruction)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);

    instruction.name = "adds";

    switch (b1 & 0xf0) {
    case 0x00:
        instruction.op1.set_immediate8(1);
        break;
    case 0x80:
        instruction.op1.set_immediate8(2);
        break;
    case 0x90:
        instruction.op1.set_immediate8(4);
        break;
    default:
        return;
    }

    instruction.op2.set_register_direct32(b1 & 0x0f);

    instruction.parser = adds_parse;
    instruction.runner = adds_run;
}

int adds_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op2.get_register_direct32());

    reg.set(reg.get() + instruction.op1.get_immediate8());
    h8->cpu.pc() += 2;

    return 0;
}

}
}
