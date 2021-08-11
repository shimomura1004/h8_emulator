#include "subs.h"

namespace h8instructions {
namespace subs {

void subs_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "subs";
    instruction.op1.set_register_direct32(b1 & 0x0f);

    switch (b1 & 0xf0)
    {
    case 0x00:
        instruction.op2.set_immediate8(1);
        break;
    case 0x80:
        instruction.op2.set_immediate8(2);
        break;
    case 0x90:
        instruction.op2.set_immediate8(4);
        break;
    default:
        return;
    }

    instruction.parser = subs_parse;
    instruction.runner = subs_run;
}

int subs_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());

    int32_t dst_value = reg.get();
    int32_t result_value = dst_value - instruction.op2.get_immediate8();
    reg.set(result_value);

    // ccr は変化しない

    h8->cpu.pc() += 2;

    return 0;
}

}
}
