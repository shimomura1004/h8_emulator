#include "exts.h"

int h8instructions::exts::exts_w(H8300H* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (b1 & 0x0f);
    Register32& reg = h8->reg[register_index];

    unsigned char rl = reg.get_rl();
    unsigned char rl_msb = rl >> 7;

    if (rl_msb) {
        // 最上位ビットが1なので負数
        reg.set_rh(0xff);
        h8->ccr.set_n();
    } else {
        reg.set_rh(0x00);
        h8->ccr.clear_n();
    }

    if (reg.get_r() == 0) {
        h8->ccr.set_z();        
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}

int h8instructions::exts::exts_l(H8300H* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = (b1 & 0x07);
    Register32& reg = h8->reg[register_index];

    unsigned char r = reg.get_r();
    unsigned char r_msb = r >> 15;

    if (r_msb) {
        // 最上位ビットが1なので負数
        reg.set_r(0xffff);
        h8->ccr.set_n();
    } else {
        reg.set_r(0x0000);
        h8->ccr.clear_n();
    }

    if (reg.get_e() == 0) {
        h8->ccr.set_z();        
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}
