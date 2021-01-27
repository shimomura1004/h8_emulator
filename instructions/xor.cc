#include "xor.h"

template<class T>
static void update_ccr(H8300H* h8, T value)
{
    if (value < 0) {
        h8->ccr.set_n();
    } else {
        h8->ccr.clear_n();
    }

    if (value == 0) {
        h8->ccr.set_z();
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();
}

int h8instructions::xorl::xor_immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t dst_reg_index = (b0 & 0x0f);
    Register8& dst = h8->reg8[dst_reg_index];

    uint8_t src_value = h8->fetch_instruction_byte(1);
    uint8_t dst_value = dst.get();
    uint8_t result_value = src_value ^ dst_value;

    dst.set(result_value);

    update_ccr<int8_t>(h8, (int8_t)result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::xorl::xor_register_direct_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_reg_index = (b1 & 0xf0) >> 4;
    uint8_t dst_reg_index = (b1 & 0x0f);
    const Register8& src = h8->reg8[src_reg_index];
    Register8& dst = h8->reg8[dst_reg_index];

    uint8_t src_value = src.get();
    uint8_t dst_value = dst.get();
    uint8_t result_value = src_value ^ dst_value;

    dst.set(result_value);

    update_ccr<int8_t>(h8, (int8_t)result_value);

    h8->pc += 2;

    return 0;
}
