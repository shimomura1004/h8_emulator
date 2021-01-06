#include "mov.h"

int h8instructions::mov::reg_to_reg(H8300H* h8)
{
    unsigned char b0 = h8->fetch_instruction_byte(0);

    return -1;
}

int h8instructions::mov::location_to_reg(H8300H* h8);
int h8instructions::mov::reg_to_location(H8300H* h8);
