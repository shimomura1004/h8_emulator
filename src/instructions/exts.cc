#include "exts.h"

namespace h8instructions {
namespace exts {

void exts_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "exts.w";
    instruction.op1.set_register_direct16(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = exts_w_parse;
    instruction.runner = exts_w_run;
}

int exts_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& reg = h8->cpu.reg16(instruction.op1.get_register_direct16());

    // 一度1バイトにして、その後に2バイトに符号拡張させる
    int16_t value = (int8_t)reg.get();

    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

void exts_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "exts.l";
    instruction.op1.set_register_direct32(b1 & 0x07);
    instruction.op2.set_not_used();

    instruction.parser = exts_l_parse;
    instruction.runner = exts_l_run;
}

int exts_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());

    // 一度2バイトにして、その後に4バイトに符号拡張させる
    int32_t value = (int16_t)reg.get();

    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
