#include "not.h"

int h8instructions::notl::not_b(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register8& reg = h8->reg8[register_index];

    uint8_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}

int h8instructions::notl::not_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->reg16[register_index];

    uint16_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}

int h8instructions::notl::not_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    Register32& reg = h8->reg[register_index];

    uint32_t value = reg.get();
    value = ~value;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}
