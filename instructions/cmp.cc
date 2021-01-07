#include "cmp.h"

static void update_ccr(H8300H* h8, int8_t value)
{
    // todo: update H

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

    // todo: update V
    // todo: update C
}

static int immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    Register32& reg = h8->reg[register_index % 8];

    int8_t imm = h8->fetch_instruction_byte(1);
    int8_t left = (register_index < 8)
                    ? reg.get_rh()
                    : reg.get_rl();
    int8_t value = left - imm;

    update_ccr(h8, value);
    h8->pc += 2;

    return 0;
}

static int register_indirect_b(H8300H* h8)
{
    return -1;
}

int h8instructions::cmp::cmp_b(H8300H *h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    if (((b0 & 0xf0) >> 4) == 0x0A) {
        return immediate_b(h8);
    } else if (b0 == 0x1c) {
        return register_indirect_b(h8);
    } else {
        return -1;
    }
}
