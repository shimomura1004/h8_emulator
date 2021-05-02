#include "and.h"

static void update_ccr(H8300H* h8, uint32_t value)
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

int h8instructions::andl::and_immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t reg_index = b0 & 0x0f;
    Register8& reg = h8->cpu.reg8(reg_index);

    uint8_t imm = h8->fetch_instruction_byte(1);

    uint8_t value = reg.get() & imm;
    reg.set(value);
    update_ccr(h8, value);

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::andl::and_immediate_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x0f;
    Register16& reg = h8->cpu.reg16(reg_index);

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    uint16_t imm = *(uint16_t*)immediate;

    uint16_t value = reg.get() & imm;
    reg.set(value);
    update_ccr(h8, value);

    h8->cpu.pc() += 4;

    return 0;
}

int h8instructions::andl::and_immediate_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x07;
    Register32& reg = h8->cpu.reg32(reg_index);

    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    uint32_t imm = *(uint32_t*)immediate;

    uint32_t value = reg.get() & imm;
    reg.set(value);
    update_ccr(h8, value);

    h8->cpu.pc() += 6;

    return 0;
}
