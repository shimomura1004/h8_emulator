#include "sub.h"

namespace h8instructions {
namespace sub {

void register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "sub.b";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = register_direct_b_parse;
    instruction.runner = register_direct_b_run;
}

int register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    // RnH(0~7),RnL(8~f) だが ER レジスタは8本しかない
    // インデックス 0(R0H) と 8(R8L) はどちらも ER0 に対応するので、剰余を取る
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int8_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

void register_direct_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "sub.w";
    instruction.op1.set_register_direct16((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = register_direct_w_parse;
    instruction.runner = register_direct_w_run;
}

int register_direct_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int16_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

void register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "sub.l";
    instruction.op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = register_direct_l_parse;
    instruction.runner = register_direct_l_run;
}

int register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int32_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

void immediate_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    // H8 仕様書が間違っている…即値は4バイト
    int32_t immediate = h8instructions::parse_immediate<int32_t>(h8, 2);

    instruction.name = "sub.l";
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = immediate_l_parse;
    instruction.runner = immediate_l_run;
}

int immediate_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());
    int32_t src_value = instruction.op1.get_immediate32();

    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int32_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 6;

    return 0;
}

}
}
