#include "andc.h"

int h8instructions::andc::andc(H8300H* h8)
{
    uint8_t imm = h8->fetch_instruction_byte(1);

    (h8->ccr.i()  && (imm & (1<<7))) ? h8->ccr.set_i()  : h8->ccr.clear_i();
    (h8->ccr.ui() && (imm & (1<<6))) ? h8->ccr.set_ui() : h8->ccr.clear_ui();
    (h8->ccr.h()  && (imm & (1<<5))) ? h8->ccr.set_h()  : h8->ccr.clear_h();
    (h8->ccr.u()  && (imm & (1<<4))) ? h8->ccr.set_u()  : h8->ccr.clear_u();
    (h8->ccr.n()  && (imm & (1<<3))) ? h8->ccr.set_n()  : h8->ccr.clear_n();
    (h8->ccr.z()  && (imm & (1<<2))) ? h8->ccr.set_z()  : h8->ccr.clear_z();
    (h8->ccr.v()  && (imm & (1<<1))) ? h8->ccr.set_v()  : h8->ccr.clear_v();
    (h8->ccr.c()  && (imm & (1<<0))) ? h8->ccr.set_c()  : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}
