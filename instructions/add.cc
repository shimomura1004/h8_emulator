#include "add.h"

template<int n, class T>
static void update_ccr(H8300H* h8, T src_value, T dst_value, T result_value)
{
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = (src_value_nth_bit &&  dst_value_nth_bit) ||
             (dst_value_nth_bit && !result_value_nth_bit) ||
             (src_value_nth_bit && !result_value_nth_bit);
    h ? h8->ccr.set_h() : h8->ccr.clear_h();

    result_value < 0 ? h8->ccr.set_n() : h8->ccr.clear_n();
    result_value == 0 ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = ( src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             (!src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->ccr.set_v() : h8->ccr.clear_v();

    bool c = (src_value_mth_bit &&  dst_value_mth_bit) ||
             (dst_value_mth_bit && !result_value_mth_bit) ||
             (src_value_mth_bit && !result_value_mth_bit);
    c ? h8->ccr.set_c() : h8->ccr.clear_v();
}

// todo: 消す
int h8instructions::add::add_immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t dst_register_index = (b0 & 0x0f);
    Register8& dst = h8->reg8[dst_register_index];

    int8_t src_value = h8->fetch_instruction_byte(1);
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;
    dst.set(result_value);

    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

void h8instructions::add::add_immediate_b_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction->name = "add.b";
    instruction->op1.s8 = h8->fetch_instruction_byte(1);
    instruction->op1.mode = addressing_mode_t::Immediate8;
    instruction->op2.u8 = b0 & 0x0f;
    instruction->op2.mode = addressing_mode_t::RegisterDirect8;

    instruction->parser = h8instructions::add::add_immediate_b_parse;
    instruction->runner = h8instructions::add::add_immediate_b_run;
}

int h8instructions::add::add_immediate_b_run(H8300H* h8, Instruction* instruction)
{
    Register8& dst = h8->reg8[instruction->op2.u8];
    int8_t src_value = instruction->op1.s8;
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

// todo: 消す
int h8instructions::add::add_register_direct_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_reg_index = (b1 & 0xf0) >> 4;
    uint8_t dst_reg_index = (b1 & 0x0f);
    const Register8& src = h8->reg8[src_reg_index];
    Register8& dst = h8->reg8[dst_reg_index];

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;

    dst.set(result_value);

    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

void h8instructions::add::add_register_direct_b_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction->name = "add.b";
    instruction->op1.u8 = (b1 & 0xf0) >> 4;
    instruction->op1.mode = addressing_mode_t::RegisterDirect8;
    instruction->op2.u8 = (b1 & 0x0f);
    instruction->op1.mode = addressing_mode_t::RegisterDirect8;

    instruction->parser = h8instructions::add::add_register_direct_b_parse;
    instruction->runner = h8instructions::add::add_register_direct_b_run;
}

int h8instructions::add::add_register_direct_b_run(H8300H* h8, Instruction* instruction)
{
    const Register8& src = h8->reg8[instruction->op1.u8];
    Register8& dst = h8->reg8[instruction->op2.u8];

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

// todo: 消す
int h8instructions::add::add_immediate_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = (b1 & 0x0f);
    Register16& dst = h8->reg16[dst_register_index];

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    int16_t src_value = *(int16_t*)immediate;

    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);

    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);

    h8->pc += 4;

    return 0;
}

void h8instructions::add::add_immediate_w_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm[2];
    imm[1] = h8->fetch_instruction_byte(2);
    imm[0] = h8->fetch_instruction_byte(3);
    int16_t immediate = *(int16_t*)imm;

    instruction->name = "add.w";
    instruction->op1.s16 = immediate;
    instruction->op1.mode = addressing_mode_t::Immediate16;
    instruction->op2.u8 = b1 & 0x0f;
    instruction->op2.mode = addressing_mode_t::RegisterDirect16;

    instruction->parser = h8instructions::add::add_immediate_w_parse;
    instruction->runner = h8instructions::add::add_immediate_w_run;
}

int h8instructions::add::add_immediate_w_run(H8300H* h8, Instruction* instruction)
{
    Register16& dst = h8->reg16[instruction->op2.u8];

    int16_t src_value = instruction->op1.s16;
    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->pc += 4;

    return 0;
}

// todo: 消す
int h8instructions::add::add_register_direct_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_reg_index = (b1 & 0xf0) >> 4;
    uint8_t dst_reg_index = (b1 & 0x0f);
    const Register16& src = h8->reg16[src_reg_index];
    Register16& dst = h8->reg16[dst_reg_index];

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);

    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

void h8instructions::add::add_register_direct_w_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction->name = "add.w";
    instruction->op1.s8 = (b1 & 0xf0) >> 4;
    instruction->op1.mode = addressing_mode_t::RegisterDirect16;
    instruction->op2.u8 = (b1 & 0x0f);
    instruction->op2.mode = addressing_mode_t::RegisterDirect16;

    instruction->parser = h8instructions::add::add_register_direct_w_parse;
    instruction->runner = h8instructions::add::add_register_direct_w_run;
}

int h8instructions::add::add_register_direct_w_run(H8300H* h8, Instruction* instruction)
{
    const Register16& src = h8->reg16[instruction->op1.u8];
    Register16& dst = h8->reg16[instruction->op2.u8];

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

// todo: 消す
int h8instructions::add::add_immediate_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& dst = h8->reg[dst_register_index];

    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t src_value = *(int32_t*)immediate;

    int32_t dst_value = dst.get();
    int32_t result_value = src_value + dst_value;
    dst.set(result_value);

    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 6;

    return 0;
}

void h8instructions::add::add_immediate_l_parse(H8300H* h8, Instruction* instruction)
{

}

int h8instructions::add::add_immediate_l_run(H8300H* h8, Instruction* instruction)
{

}

// todo: 消す
int h8instructions::add::add_register_direct_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    const Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value + src_value;
    dst.set(result_value);

    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

void h8instructions::add::add_register_direct_l_parse(H8300H* h8, Instruction* instruction)
{

}

int h8instructions::add::add_register_direct_l_run(H8300H* h8, Instruction* instruction)
{

}
