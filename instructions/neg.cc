#include "neg.h"

template<int n, class T>
static void update_ccr(H8300H* h8, T dst_value, T result_value)
{
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = (dst_value_nth_bit || result_value_nth_bit);
    h ? h8->ccr.set_h() : h8->ccr.clear_h();

    result_value < 0 ? h8->ccr.set_n() : h8->ccr.clear_n();
    result_value == 0 ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = dst_value_mth_bit &&  result_value_mth_bit;
    v ? h8->ccr.set_v() : h8->ccr.clear_v();

    bool c = dst_value_mth_bit || result_value_mth_bit;
    c ? h8->ccr.set_c() : h8->ccr.clear_v();
}

int h8instructions::neg::neg_w(H8300H* h8)
{
    int8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = b1 & 0x0f;
    Register32& dst = h8->reg[dst_register_index % 8];

    int16_t dst_value = (dst_register_index < 8) ? dst.get_r() : dst.get_e();
    int16_t result_value = -dst_value;
    if (dst_value == (int16_t)0x8000) {
        // 「実行前のRdの内容がH'8000の場合 の結果はH'8000となります」
        result_value = dst_value;
    }

    update_ccr<16, int16_t>(h8, dst_value, result_value);

    h8->pc += 2;

    return 0;
}
