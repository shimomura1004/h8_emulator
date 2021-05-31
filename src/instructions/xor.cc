#include "xor.h"
#include "arithmetic_helper.h"

namespace h8instructions {
namespace xorl {

// TODO: 関数名に xor_ は不要
void xor_immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_immediate_b<xor_immediate_b_parse, xor_immediate_b_run>(h8, instruction, "xor.b");
}

int xor_immediate_b_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_b(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 ^ op2;});
}

void xor_register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "xor.b";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = h8instructions::xorl::xor_register_direct_b_parse;
    instruction.runner = h8instructions::xorl::xor_register_direct_b_run;
}

int xor_register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t src_value = src.get();
    uint8_t dst_value = dst.get();
    uint8_t result_value = src_value ^ dst_value;

    dst.set(result_value);
    h8instructions::update_ccr_nzv<int8_t>(h8, (int8_t)result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void xor_register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);

    instruction.name = "xor.l";
    instruction.op1.set_register_direct32((b3 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b3 & 0x07);

    instruction.parser = h8instructions::xorl::xor_register_direct_l_parse;
    instruction.runner = h8instructions::xorl::xor_register_direct_l_run;
}

int xor_register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    uint32_t src_value = src.get();
    uint32_t dst_value = dst.get();
    uint32_t result_value = src_value ^ dst_value;

    dst.set(result_value);
    h8instructions::update_ccr_nzv<int32_t>(h8, (int32_t)result_value);
    h8->cpu.pc() += 4;

    return 0;
}

}
}
