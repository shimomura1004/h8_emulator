#include "and.h"
#include "xor.h"

// TODO: and と xor は演算子以外ほぼ同じ

int h8instructions::andl::and_immediate_w(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x0f;
    Register16& reg = h8->cpu.reg16(reg_index);

    uint16_t imm = parse_immediate<uint16_t>(h8, 2);

    uint16_t value = reg.get() & imm;
    reg.set(value);
    h8instructions::update_ccr_nzv<int16_t>(h8, value);

    h8->cpu.pc() += 4;

    return 0;
}

int h8instructions::andl::and_immediate_l(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x07;
    Register32& reg = h8->cpu.reg32(reg_index);

    uint32_t imm = h8instructions::parse_immediate<uint32_t>(h8, 2);

    uint32_t value = reg.get() & imm;
    reg.set(value);
    h8instructions::update_ccr_nzv<int32_t>(h8, value);

    h8->cpu.pc() += 6;

    return 0;
}


namespace h8instructions {
namespace andl {

void and_immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = "and.b";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::andl::and_immediate_b_parse;
    instruction.runner = h8instructions::andl::and_immediate_b_run;
}

int and_immediate_b_run(H8Board *h8, Instruction& instruction)
{
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t src_value = instruction.op1.get_immediate8();
    uint8_t dst_value = dst.get();
    uint8_t result_value = src_value & dst_value;

    dst.set(result_value);
    h8instructions::update_ccr_nzv<int8_t>(h8, (int8_t)result_value);
    h8->cpu.pc() += 2;

    return 0;
}


}
}
