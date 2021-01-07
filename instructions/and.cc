#include "and.h"

static void update_ccr(H8300H* h8, uint32_t value)
{
    if (value < 0) {
        h8->ccr.set_n();
    } else {
        h8->ccr.clear_n();
    }

    if (value == 0) {
        h8->ccr.set_z();
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();
}

int h8instructions::andl::and_immediate_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = b1 & 0x0f;
    Register32& reg = h8->reg[reg_index % 8];

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    uint16_t imm = *(uint16_t*)immediate;

    if (reg_index < 8) {
        uint16_t value = reg.get_r() & imm;
        reg.set_r(value);
        update_ccr(h8, value);
    } else {
        uint16_t value = reg.get_e() & imm;
        reg.set_e(value);
        update_ccr(h8, value);
    }

    h8->pc += 4;

    return 0;
}
