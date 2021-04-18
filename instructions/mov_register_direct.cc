#include "mov_register_direct.h"
#include "mov.h"

int h8instructions::mov::register_direct_b(H8300H* h8)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);
    const uint8_t src_register_index = (b1 & 0xf0) >> 4;
    const uint8_t dst_register_index = (b1 & 0x0f);
    const Register8& src = h8->reg8[src_register_index];
    Register8& dst = h8->reg8[dst_register_index];

    uint8_t value = src.get();
    dst.set(value);

    update_ccr<int8_t>(h8, value);
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::register_direct_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    const Register16& src = h8->reg16[src_register_index];
    Register16& dst = h8->reg16[dst_register_index];

    uint16_t value = src.get();
    dst.set(value);

    update_ccr<int16_t>(h8, value);
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::register_direct_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    dst.set(src.get());
    update_ccr<int32_t>(h8, src.get());
    h8->pc += 2;

    return 0;
}
