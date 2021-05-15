#include "not.h"

// TODO: ccr 更新部の共通化

int h8instructions::notl::not_b(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register8& reg = h8->cpu.reg8(register_index);

    uint8_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::notl::not_w(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->cpu.reg16(register_index);

    uint16_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::notl::not_l(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    Register32& reg = h8->cpu.reg32(register_index);

    uint32_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}
