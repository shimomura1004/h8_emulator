#include "not.h"

// TODO: ccr 更新部の共通化, b/w/l の共通化

namespace h8instructions {
namespace h8not {

void not_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "not.b";
    instruction.op1.set_register_direct8(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = not_b_parse;
    instruction.runner = not_b_run;
}

int not_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& reg = h8->cpu.reg8(instruction.op1.get_register_direct8());

    uint8_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

void not_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "not.w";
    instruction.op1.set_register_direct16(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = not_w_parse;
    instruction.runner = not_w_run;
}

int not_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& reg = h8->cpu.reg16(instruction.op1.get_register_direct16());

    uint16_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

void not_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "not.l";
    instruction.op1.set_register_direct32(b1 & 0x07);
    instruction.op2.set_not_used();

    instruction.parser = not_l_parse;
    instruction.runner = not_l_run;
}

int not_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());

    uint32_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
