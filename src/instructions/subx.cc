#include "subx.h"
#include "sub.h"

namespace h8instructions {
namespace subx {

void register_direct_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    
    instruction.name = "subx";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = register_direct_parse;
    instruction.runner = register_direct_run;
}

int register_direct_run(H8Board* h8, Instruction& instruction)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    char src_value = src.get();
    char dst_value = dst.get();
    char result_value = dst_value - src_value - (h8->cpu.ccr().c() ? 1 : 0);
    dst.set(result_value);

    bool prev_z = h8->cpu.ccr().z();
    sub::update_ccr<int8_t>(h8, src_value, dst_value, result_value);
    if (result_value == 0) {
        prev_z ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    }

    h8->cpu.pc() += 2;

    return 0;
}

}
}
