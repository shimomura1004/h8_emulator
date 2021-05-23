#include "cmp.h"
#include "sub.h"

namespace h8instructions {
namespace cmp {

void cmp_immediate_b_parse(H8Board *h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t b1 = h8->fetch_instruction_byte(1);

    // #xx:8,Rd
    instruction.name ="cmp.b";
    instruction.op1.set_immediate8(b1);
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::cmp::cmp_immediate_b_parse;
    instruction.runner = h8instructions::cmp::cmp_immediate_b_run;
}

int cmp_immediate_b_run(H8Board *h8, Instruction& instruction)
{
    const Register8 reg = h8->cpu.reg8(instruction.op2.get_register_direct8());

    int8_t src_value = instruction.op1.get_immediate8();
    int8_t dst_value = reg.get();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void cmp_register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name ="cmp.b";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = h8instructions::cmp::cmp_register_direct_b_parse;
    instruction.runner = h8instructions::cmp::cmp_register_direct_b_run;
}

int cmp_register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    const Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void cmp_immediate_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    uint8_t imm[2];
    imm[1] = h8->fetch_instruction_byte(2);
    imm[0] = h8->fetch_instruction_byte(3);
    int16_t immediate = *(int16_t*)imm;

    // #xx:8,Rd
    instruction.name ="cmp.w";
    instruction.op1.set_immediate16(immediate);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::cmp::cmp_immediate_w_parse;
    instruction.runner = h8instructions::cmp::cmp_immediate_w_run;
}

int cmp_immediate_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16 reg = h8->cpu.reg16(instruction.op2.get_register_direct16());

    int16_t src_value = instruction.op1.get_immediate16();
    int16_t dst_value = reg.get();
    int16_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 4;

    return 0;
}

void cmp_register_direct_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name ="cmp.w";
    instruction.op1.set_register_direct16((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::cmp::cmp_register_direct_w_parse;
    instruction.runner = h8instructions::cmp::cmp_register_direct_w_run;
}

int cmp_register_direct_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    const Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

void cmp_immediate_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    uint8_t imm[4];
    imm[3] = h8->fetch_instruction_byte(2);
    imm[2] = h8->fetch_instruction_byte(3);
    imm[1] = h8->fetch_instruction_byte(4);
    imm[0] = h8->fetch_instruction_byte(5);
    int32_t immediate = *(int32_t*)imm;

    instruction.name ="cmp.l";
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = h8instructions::cmp::cmp_immediate_l_parse;
    instruction.runner = h8instructions::cmp::cmp_immediate_l_run;
}

int cmp_immediate_l_run(H8Board* h8, Instruction& instruction)
{
    const Register32& reg = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int32_t src_value = instruction.op1.get_immediate32();
    int32_t dst_value = reg.get();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 6;

    return 0;
}

void cmp_register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name ="cmp.l";
    instruction.op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = h8instructions::cmp::cmp_register_direct_l_parse;
    instruction.runner = h8instructions::cmp::cmp_register_direct_l_run;
}

int cmp_register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    const Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

}
}
