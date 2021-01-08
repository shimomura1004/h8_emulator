#include "sub.h"

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

    update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);

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

    update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);

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

    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::sub::sub_immediate_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& dst = h8->reg[dst_register_index];

    // H8 仕様書が間違っている…
    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t src_value = *(int32_t*)immediate;

    int32_t dst_value = dst.get_er();
    int32_t result_value = dst_value - src_value;
    dst.set_er(result_value);

    update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);

    h8->pc += 6;

    return 0;
}
