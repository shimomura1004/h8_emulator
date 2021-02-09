#include "bld.h"

int h8instructions::bld::bld_register_direct(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = (b1 & 0x0f);
    uint8_t imm = (b1 & 0x70) >> 4;
    const Register8& reg = h8->reg8[reg_index];

    bool nth_bit = reg.get() & (1 << imm);

    if (nth_bit) {
        h8->ccr.set_c();
    } else {
        h8->ccr.clear_c();
    }

    h8->pc += 2;

    return 0;
}
