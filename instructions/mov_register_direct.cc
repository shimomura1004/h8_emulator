#include "mov_register_direct.h"
#include "mov.h"

int h8instructions::mov::register_direct_b(H8300H* h8)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);
    const uint8_t src_register_index = (b1 & 0xf0) >> 4;
    const uint8_t dst_register_index = (b1 & 0x0f);
    const Register8& src = h8->reg8[src_register_index];
    Register8& dst = h8->reg8[dst_register_index];

    uint8_t value = src.get();
    dst.set(value);

    update_ccr<int8_t>(h8, value);
    h8->pc += 2;

    return 0;
}

void h8instructions::mov::register_direct_b_parse(H8300H* h8, Instruction* instruction)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction->name = "mov.b";
    instruction->op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction->op2.set_register_direct8(b1 & 0x0f);

    instruction->parser = h8instructions::mov::register_direct_b_parse;
    instruction->runner = h8instructions::mov::register_direct_b_run;
}

int h8instructions::mov::register_direct_b_run(H8300H* h8, Instruction* instruction)
{
    const Register8& src = h8->reg8[instruction->op1.get_register_direct8()];
    Register8& dst = h8->reg8[instruction->op2.get_register_direct8()];

    uint8_t value = src.get();

    dst.set(value);
    update_ccr<int8_t>(h8, value);
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::register_direct_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    const Register16& src = h8->reg16[src_register_index];
    Register16& dst = h8->reg16[dst_register_index];

    uint16_t value = src.get();
    dst.set(value);

    update_ccr<int16_t>(h8, value);
    h8->pc += 2;

    return 0;
}

void h8instructions::mov::register_direct_w_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction->name = "mov.w";
    instruction->op1.set_register_direct16((b1 & 0xf0) >> 4);
    instruction->op2.set_register_direct16(b1 & 0x0f);

    instruction->parser = h8instructions::mov::register_direct_w_parse;
    instruction->runner = h8instructions::mov::register_direct_w_run;
}

int h8instructions::mov::register_direct_w_run(H8300H* h8, Instruction* instruction)
{
    const Register16& src = h8->reg16[instruction->op1.get_register_direct16()];
    Register16& dst = h8->reg16[instruction->op2.get_register_direct16()];

    uint16_t value = src.get();

    dst.set(value);
    update_ccr<int16_t>(h8, value);
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::register_direct_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    dst.set(src.get());
    update_ccr<int32_t>(h8, src.get());

    h8->pc += 2;

    return 0;
}

void h8instructions::mov::register_direct_l_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction->name = "mov.l";
    instruction->op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction->op2.set_register_direct32(b1 & 0x07);

    instruction->parser = h8instructions::mov::register_direct_l_parse;
    instruction->runner = h8instructions::mov::register_direct_l_run;
}

int h8instructions::mov::register_direct_l_run(H8300H* h8, Instruction* instruction)
{
    const Register32& src = h8->reg[instruction->op1.get_register_direct32()];
    Register32& dst = h8->reg[instruction->op2.get_register_direct32()];

    uint32_t value = src.get();

    dst.set(value);
    update_ccr<int32_t>(h8, value);
    h8->pc += 2;

    return 0;
}
