#include "or.h"

static void update_ccr(H8Board* h8, uint32_t value)
{
    if (value < 0) {
        h8->cpu.ccr().set_n();
    } else {
        h8->cpu.ccr().clear_n();
    }

    if (value == 0) {
        h8->cpu.ccr().set_z();
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();
}

int h8instructions::orl::or_immediate_w(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x0f;
    Register16& reg = h8->cpu.reg16(reg_index);

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    uint16_t imm = *(uint16_t*)immediate;

    uint16_t value = reg.get() | imm;
    reg.set(value);
    update_ccr(h8, value);

    h8->cpu.pc() += 4;

    return 0;
}

namespace h8instructions {
namespace orl {

void immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = "or.b";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = h8instructions::orl::immediate_b_parse;
    instruction.runner = h8instructions::orl::immediate_b_run;
}

int immediate_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& reg = h8->cpu.reg8(instruction.op2.get_register_direct8());
    uint8_t imm = instruction.op1.get_immediate8();

    uint8_t value = reg.get() | imm;
    reg.set(value);
    update_ccr(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}

}
}