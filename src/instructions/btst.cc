#include "btst.h"

int h8instructions::btst::btst_register_direct_imm(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    const Register8& dst = h8->cpu.reg8(dst_register_index);

    uint8_t mask = (1 << imm);
    uint8_t value = dst.get();

    (value & mask) ? h8->cpu.ccr().clear_z() : h8->cpu.ccr().set_z();

    h8->cpu.pc() += 2;

    return 0;
}
