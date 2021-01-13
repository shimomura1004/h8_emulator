#include "mulxs.h"

int h8instructions::mulxs::mulxs_b(H8300H* h8)
{
    return -1;
}

int h8instructions::mulxs::mulxs_w(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_reg_index = (b3 & 0xf0) >> 4;
    uint8_t dst_reg_index = (b3 & 0x07);
    const Register32& src = h8->reg[src_reg_index % 8];
    Register32& dst = h8->reg[dst_reg_index];

    int16_t src_value = (src_reg_index < 8) ? src.get_r() : src.get_e();
    // ER の下位16ビット(= R)を抜き出す
    int16_t dst_value = dst.get_r();
    int32_t result_value = dst_value * src_value;

    dst.set_er(result_value);

    (result_value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (result_value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();

    h8->pc += 4;

    return 0;
}
