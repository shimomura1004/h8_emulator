#include "bset.h"

int h8instructions::bset::bset_register_direct_imm(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    Register32& dst = h8->reg[dst_register_index % 8];

    uint8_t mask = (1 << imm);
    uint8_t value = (dst_register_index < 8) ? dst.get_rh() : dst.get_rl();
    value |= mask;
    (dst_register_index < 8) ? dst.set_rh(value) : dst.set_rl(value);

    h8->pc += 2;

    return 0;
}
