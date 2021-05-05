#include "mulxs.h"

int h8instructions::mulxs::mulxs_b(H8Board* h8)
{
    return -1;
}

int h8instructions::mulxs::mulxs_w(H8Board* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_reg_index = (b3 & 0xf0) >> 4;
    uint8_t dst_reg_index = (b3 & 0x07);
    const Register16& src = h8->cpu.reg16(src_reg_index);
    Register32& dst = h8->cpu.reg32(dst_reg_index);

    int16_t src_value = src.get();
    // ER の下位16ビット(= R)を抜き出す
    int16_t dst_value = dst.get() & 0x0000ffff;
    int32_t result_value = (int32_t)dst_value * src_value;

    dst.set(result_value);

    (result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    h8->cpu.pc() += 4;

    return 0;
}
