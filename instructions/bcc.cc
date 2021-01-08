#include "bcc.h"

int h8instructions::bcc::bra_8(H8300H* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2 + disp;
    return 0;
}

int h8instructions::bcc::bne_8(H8300H *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    if (!h8->ccr.z()) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::beq_8(H8300H *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    if (h8->ccr.z()) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::ble_8(H8300H *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    bool n_xor_v = (h8->ccr.n() && !h8->ccr.v()) || (!h8->ccr.n() && h8->ccr.v());
    if (h8->ccr.z() || n_xor_v) {
        h8->pc += disp;
    }

    return 0;
}