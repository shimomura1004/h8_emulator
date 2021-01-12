#include "bcc.h"

int h8instructions::bcc::bra_8(H8300H* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;
    h8->pc += disp;

    return 0;
}

int h8instructions::bcc::bra_16(H8300H* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->pc += 4;
    h8->pc += disp;

    return 0;
}

int h8instructions::bcc::bls_8(H8300H* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    if (h8->ccr.c() || h8->ccr.z()) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::bcc_8(H8300H* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    if (!h8->ccr.c()) {
        h8->pc += disp;
    }

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

int h8instructions::bcc::bne_16(H8300H* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->pc += 4;

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

int h8instructions::bcc::beq_16(H8300H *h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->pc += 4;

    if (h8->ccr.z()) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::bge_8(H8300H *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    bool n_xor_v = (h8->ccr.n() && !h8->ccr.v()) || (!h8->ccr.n() && h8->ccr.v());
    if (!n_xor_v) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::blt_16(H8300H* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->pc += 4;

    bool n_xor_v = (h8->ccr.n() && !h8->ccr.v()) || (!h8->ccr.n() && h8->ccr.v());
    if (n_xor_v) {
        h8->pc += disp;
    }

    return 0;
}

int h8instructions::bcc::bgt_8(H8300H* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->pc += 2;

    bool n_xor_v = (h8->ccr.n() && !h8->ccr.v()) || (!h8->ccr.n() && h8->ccr.v());
    if (!(h8->ccr.z() || n_xor_v)) {
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
