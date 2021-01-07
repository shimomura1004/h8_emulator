#include "bcc.h"

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
