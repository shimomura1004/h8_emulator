#include "shll.h"

// todo: shll は1つにまとめられそう

int h8instructions::shll::shll_b(H8Board *h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (b1 & 0x0f);
    Register8& reg = h8->cpu.reg8(register_index);

    uint8_t er = reg.get();
    bool er_msb = er & 0x80;
    er = er << 1;

    reg.set(er);

    (er & 0x80) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (er == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    er_msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::shll::shll_w(H8Board *h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (b1 & 0x0f);
    Register16& reg = h8->cpu.reg16(register_index);

    uint16_t er = reg.get();
    bool er_msb = er & 0x8000;
    er = er << 1;

    reg.set(er);

    (er & 0x8000) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (er == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    er_msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::shll::shll_l(H8Board *h8)
{
    unsigned char instruction_byte_1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (instruction_byte_1 & 0x0f);
    Register32& reg = h8->cpu.reg32(register_index);

    uint32_t er = reg.get();
    bool er_msb = er & 0x80000000;
    er = er << 1;

    reg.set(er);

    (er & 0x80000000) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (er == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    er_msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}
