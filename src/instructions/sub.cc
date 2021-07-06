#include "sub.h"

// おそらく index 0~7 が RnH、8~f が RnL
int h8instructions::sub::sub_b(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);

    // RnH(0~7),RnL(8~f) だが ER レジスタは8本しかない
    // インデックス 0(R0H) と 8(R8L) はどちらも ER0 に対応するので、剰余を取る
    const Register8& src = h8->cpu.reg8(src_register_index);
    Register8& dst = h8->cpu.reg8(dst_register_index);

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int8_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::sub::sub_w(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);

    const Register16& src = h8->cpu.reg16(src_register_index);
    Register16& dst = h8->cpu.reg16(dst_register_index);

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int16_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::sub::sub_l(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& src = h8->cpu.reg32(src_register_index);
    Register32& dst = h8->cpu.reg32(dst_register_index);

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - src_value;
    dst.set(result_value);

    h8instructions::sub::update_ccr<int32_t>(h8, src_value, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

namespace h8instructions {
namespace sub {

void sub_immediate_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    // H8 仕様書が間違っている…即値は4バイト
    int32_t immediate = h8instructions::parse_immediate<int32_t>(h8, 2);

    instruction.name = "sub.l";
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = sub_immediate_l_parse;
    instruction.runner = sub_immediate_l_run;
}

int sub_immediate_l_run(H8Board* h8, Instruction& instruction)
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
