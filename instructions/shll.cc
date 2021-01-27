#include "shll.h"

int h8instructions::shll::shll_b(H8300H *h8)
{
    return -1;
}

int h8instructions::shll::shll_w(H8300H *h8)
{
    return -1;
}

int h8instructions::shll::shll_l(H8300H *h8)
{
    unsigned char instruction_byte_1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (instruction_byte_1 & 0x0f);
    Register32& reg = h8->reg[register_index];

    uint32_t er = reg.get();
    bool er_msb = er & 0x80000000;
    er = er << 1;

    reg.set(er);

    (er & 0x80000000) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (er == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();
    er_msb ? h8->ccr.set_c() : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}
