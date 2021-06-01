#include "and.h"
#include "arithmetic_helper.h"

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
    h8instructions::arithmetic::parse_immediate_b<and_immediate_b_parse, and_immediate_b_run>(h8, instruction, "and.b");
}

int and_immediate_b_run(H8Board *h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_b(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 & op2;});
}

}
}
