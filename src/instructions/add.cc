#include "add.h"

template<int n, class T>
static void update_ccr(H8Board* h8, T src_value, T dst_value, T result_value)
{
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = (src_value_nth_bit &&  dst_value_nth_bit) ||
             (dst_value_nth_bit && !result_value_nth_bit) ||
             (src_value_nth_bit && !result_value_nth_bit);
    h ? h8->cpu.ccr().set_h() : h8->cpu.ccr().clear_h();

    result_value < 0 ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    result_value == 0 ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = ( src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             (!src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->cpu.ccr().set_v() : h8->cpu.ccr().clear_v();

    bool c = (src_value_mth_bit &&  dst_value_mth_bit) ||
             (dst_value_mth_bit && !result_value_mth_bit) ||
             (src_value_mth_bit && !result_value_mth_bit);
    c ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_v();
}

void h8instructions::add::add_immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = "add.b";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::add::add_immediate_b_parse;
    instruction.runner = h8instructions::add::add_immediate_b_run;
}

int h8instructions::add::add_immediate_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());
    int8_t src_value = instruction.op1.get_immediate8();
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void h8instructions::add::add_register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "add.b";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = h8instructions::add::add_register_direct_b_parse;
    instruction.runner = h8instructions::add::add_register_direct_b_run;
}

int h8instructions::add::add_register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void h8instructions::add::add_immediate_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm[2];
    imm[1] = h8->fetch_instruction_byte(2);
    imm[0] = h8->fetch_instruction_byte(3);
    int16_t immediate = *(int16_t*)imm;

    instruction.name = "add.w";
    instruction.op1.set_immediate16(immediate);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::add::add_immediate_w_parse;
    instruction.runner = h8instructions::add::add_immediate_w_run;
}

int h8instructions::add::add_immediate_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());

    int16_t src_value = instruction.op1.get_immediate16();
    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 4;

    return 0;
}

void h8instructions::add::add_register_direct_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "add.w";
    instruction.op1.set_register_direct16((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::add::add_register_direct_w_parse;
    instruction.runner = h8instructions::add::add_register_direct_w_run;
}

int h8instructions::add::add_register_direct_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void h8instructions::add::add_immediate_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    uint8_t imm[4];
    imm[3] = h8->fetch_instruction_byte(2);
    imm[2] = h8->fetch_instruction_byte(3);
    imm[1] = h8->fetch_instruction_byte(4);
    imm[0] = h8->fetch_instruction_byte(5);
    int32_t immediate = *(int32_t*)imm;

    instruction.name = "add.l";
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = h8instructions::add::add_immediate_l_parse;
    instruction.runner = h8instructions::add::add_immediate_l_run;
}

int h8instructions::add::add_immediate_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int32_t src_value = instruction.op1.get_immediate32();
    int32_t dst_value = dst.get();
    int32_t result_value = src_value + dst_value;

    dst.set(result_value);
    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 6;

    return 0;
}

void h8instructions::add::add_register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "add.l";
    instruction.op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = h8instructions::add::add_register_direct_l_parse;
    instruction.runner = h8instructions::add::add_register_direct_l_run;
}

int h8instructions::add::add_register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value + src_value;

    dst.set(result_value);
    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}
