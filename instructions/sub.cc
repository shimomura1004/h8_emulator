#include "sub.h"

// todo: CCR の動作が不明確("コンディションコードの変化"に詳細があった)
// おそらく index 0~7 が RnH、8~f が RnL
int h8instructions::sub::sub_b(H8300H *h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char src_register_index = (b1 & 0xf0) >> 4;
    unsigned char dst_register_index = (b1 & 0x0f);

    // RnH(0~7),RnL(8~f) だが ER レジスタは8本しかない
    // インデックス 0(R0H) と 8(R8L) はどちらも ER0 に対応するので、剰余を取る
    Register32& src = h8->reg[src_register_index % 8];
    Register32& dst = h8->reg[dst_register_index % 8];

    char left = (dst_register_index < 8) ? dst.get_rh() : dst.get_rl();
    char right = (src_register_index < 8) ? src.get_rh() : src.get_rl();
    char value = left - right;

    (dst_register_index < 8) ? dst.set_rh(value) : dst.set_rl(value);

    // ビット3(4ビット目)にボローが発生したか？は、下位4ビットを引き算すればわかる
    if ((left % 0x0f) - (right % 0x0f) < 0) {
        h8->ccr.set_h();
    } else {
        h8->ccr.clear_h();
    }

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

    // オーバーフローが発生したか？
    if (((int16_t)left - (int16_t)right) >= 0xffff) {
        h8->ccr.set_v();
    } else {
        h8->ccr.clear_v();
    }

    // ビット7(8ビット目)にボローが発生したか？
    if (value < 0) {
        h8->ccr.set_c();
    } else {
        h8->ccr.clear_c();
    }

    h8->pc += 2;

    return 0;
}

int h8instructions::sub::sub_w(H8300H *h8)
{
    return -1;
}

int h8instructions::sub::sub_l(H8300H *h8)
{
    return -1;
}
