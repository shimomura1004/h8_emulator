#include "add.h"

template<int n, class T>
static void update_ccr(H8300H* h8, T src_value, T dst_value, T result_value)
{
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = (src_value_nth_bit &&  dst_value_nth_bit) ||
             (dst_value_nth_bit && !result_value_nth_bit) ||
             (src_value_nth_bit && !result_value_nth_bit);
    h ? h8->ccr.set_h() : h8->ccr.clear_h();

    result_value < 0 ? h8->ccr.set_n() : h8->ccr.clear_n();
    result_value == 0 ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = ( src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             (!src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->ccr.set_v() : h8->ccr.clear_v();

    bool c = (src_value_mth_bit &&  dst_value_mth_bit) ||
             (dst_value_mth_bit && !result_value_mth_bit) ||
             (src_value_mth_bit && !result_value_mth_bit);
    c ? h8->ccr.set_c() : h8->ccr.clear_v();
}

int h8instructions::add::add_immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t dst_register_index = (b0 & 0x0f);
    Register32& dst = h8->reg[dst_register_index % 8];

    int8_t src_value = h8->fetch_instruction_byte(1);
    int8_t dst_value = (dst_register_index < 8) ? dst.get_rh() : dst.get_rl();
    int8_t result_value = src_value + dst_value;
    (dst_register_index < 8) ? dst.set_rh(result_value) : dst.set_rl(result_value);

    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::add::add_immediate_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& dst = h8->reg[dst_register_index];

    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t src_value = *(int32_t*)immediate;

    int32_t dst_value = dst.get_er();
    int32_t result_value = src_value + dst_value;
    dst.set_er(result_value);

    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 6;

    return 0;
}
