#include "xor.h"

namespace h8instructions {
namespace xorl {

// TODO: 一般的な ccr の更新、他と同じ
template<class T>
static void update_ccr(H8Board* h8, T value)
{
    if (value < 0) {
        h8->cpu.ccr().set_n();
    } else {
        h8->cpu.ccr().clear_n();
    }

    if (value == 0) {
        h8->cpu.ccr().set_z();
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();
}

void xor_immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = "xor.b";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::xorl::xor_immediate_b_parse;
    instruction.runner = h8instructions::xorl::xor_immediate_b_run;
}

int xor_immediate_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t src_value = instruction.op1.get_immediate8();
    uint8_t dst_value = dst.get();
    uint8_t result_value = src_value ^ dst_value;

    dst.set(result_value);
    update_ccr<int8_t>(h8, (int8_t)result_value);
    h8->cpu.pc() += 2;

    return 0;
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
    update_ccr<int8_t>(h8, (int8_t)result_value);
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
    update_ccr<int32_t>(h8, (int32_t)result_value);
    h8->cpu.pc() += 4;

    return 0;
}

}
}
