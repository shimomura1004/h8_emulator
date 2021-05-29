#include "mov_immediate.h"

namespace h8instructions {
namespace mov {

void immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = "mov.b";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::mov::immediate_b_parse;
    instruction.runner = h8instructions::mov::immediate_b_run;
}

int immediate_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& reg = h8->cpu.reg8(instruction.op2.get_register_direct8());
    int8_t value = instruction.op1.get_immediate8();

    reg.set(value);
    h8instructions::update_ccr_nzv<int8_t>(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}

void immediate_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    int16_t immediate = h8instructions::parse_immediate<int16_t>(h8, 2);

    instruction.name = "mov.w";
    instruction.op1.set_immediate16(immediate);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = h8instructions::mov::immediate_w_parse;
    instruction.runner = h8instructions::mov::immediate_w_run;
}

int immediate_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& reg = h8->cpu.reg16(instruction.op2.get_register_direct16());
    int16_t value = instruction.op1.get_immediate16();

    reg.set(value);
    h8instructions::update_ccr_nzv<int16_t>(h8, value);
    h8->cpu.pc() += 4;

    return 0;
}

void immediate_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    int32_t immediate = h8instructions::parse_immediate<int32_t>(h8, 2);

    instruction.name = "mov.l";
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x0f);

    instruction.parser = h8instructions::mov::immediate_l_parse;
    instruction.runner = h8instructions::mov::immediate_l_run;
}

int immediate_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op2.get_register_direct32());
    int32_t value = instruction.op1.get_immediate32();

    reg.set(value);
    h8instructions::update_ccr_nzv<int32_t>(h8, value);
    h8->cpu.pc() += 6;

    return 0;
}

}
}
