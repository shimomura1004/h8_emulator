#include "sub.h"

template<int n, class T>
void update_ccr_n(H8300H* h8, T src_value, T dst_value, T result_value)
{
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = ( src_value_nth_bit && !dst_value_nth_bit) ||
             (!dst_value_nth_bit &&  result_value_nth_bit) ||
             ( src_value_nth_bit &&  result_value_nth_bit);
    h ? h8->ccr.set_h() : h8->ccr.clear_h();

    result_value < 0 ? h8->ccr.set_n() : h8->ccr.clear_n();
    result_value == 0 ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = (!src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             ( src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->ccr.set_v() : h8->ccr.clear_v();

    bool c = ( src_value_mth_bit && !dst_value_mth_bit) ||
             (!dst_value_mth_bit &&  result_value_mth_bit) ||
             ( src_value_mth_bit &&  result_value_mth_bit);
    c ? h8->ccr.set_c() : h8->ccr.clear_v();
}

// おそらく index 0~7 が RnH、8~f が RnL
int h8instructions::sub::sub_b(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);

    // RnH(0~7),RnL(8~f) だが ER レジスタは8本しかない
    // インデックス 0(R0H) と 8(R8L) はどちらも ER0 に対応するので、剰余を取る
    Register32& src = h8->reg[src_register_index % 8];
    Register32& dst = h8->reg[dst_register_index % 8];

    char src_value = (src_register_index < 8) ? src.get_rh() : src.get_rl();
    char dst_value = (dst_register_index < 8) ? dst.get_rh() : dst.get_rl();
    char result_value = dst_value - src_value;
    (dst_register_index < 8) ? dst.set_rh(result_value) : dst.set_rl(result_value);

    update_ccr_n<8, int8_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::sub::sub_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);

    // おそらく R(0~7) と E(8~F) という対応と思われる
    Register32& src = h8->reg[src_register_index % 8];
    Register32& dst = h8->reg[dst_register_index % 8];

    char src_value = (src_register_index < 8) ? src.get_r() : src.get_e();
    char dst_value = (dst_register_index < 8) ? dst.get_r() : dst.get_e();
    char result_value = dst_value - src_value;
    (dst_register_index < 8) ? dst.set_r(result_value) : dst.set_e(result_value);

    update_ccr_n<16, int16_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::sub::sub_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    int32_t src_value = src.get_er();
    int32_t dst_value = dst.get_er();
    int32_t result_value = dst_value - src_value;
    dst.set_er(result_value);

    update_ccr_n<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::sub::sub_immediate_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& dst = h8->reg[dst_register_index];

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    int16_t src_value = *(int16_t*)immediate;

    int32_t dst_value = dst.get_er();
    int32_t result_value = dst_value - src_value;
    dst.set_er(result_value);

    update_ccr_n<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 4;

    return 0;
}
