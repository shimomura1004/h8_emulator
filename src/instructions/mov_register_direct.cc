#include "mov_register_direct.h"
#include "mov.h"

void h8instructions::mov::register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "mov.b";
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = h8instructions::mov::register_direct_b_parse;
    instruction.runner = h8instructions::mov::register_direct_b_run;
}

int h8instructions::mov::register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t value = src.get();

    dst.set(value);
    update_ccr<int8_t>(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}

void h8instructions::mov::register_direct_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "mov.w";
    instruction.op1.set_register_direct16((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::mov::register_direct_w_parse;
    instruction.runner = h8instructions::mov::register_direct_w_run;
}

int h8instructions::mov::register_direct_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());

    uint16_t value = src.get();

    dst.set(value);
    update_ccr<int16_t>(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}

void h8instructions::mov::register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "mov.l";
    instruction.op1.set_register_direct32((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = h8instructions::mov::register_direct_l_parse;
    instruction.runner = h8instructions::mov::register_direct_l_run;
}

int h8instructions::mov::register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    uint32_t value = src.get();

    dst.set(value);
    update_ccr<int32_t>(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}
