#include "divxs.h"

int h8instructions::divxs::divxs_w(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_register_index = (b3 & 0xf0) >> 4;
    uint8_t dst_register_index = (b3 & 0x07);
    const Register16& src = h8->reg16[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    // 被除数
    int32_t dst_value = dst.get();
    bool dst_is_positive = dst_value >= 0;
    // 除数
    int16_t src_value = src.get();
    bool src_is_positive = src_value >= 0;

    bool quotient_is_positive = (dst_is_positive && src_is_positive) ||
                                (!dst_is_positive && !src_is_positive);

    if (src_value == 0) {
        // 0除算の場合、結果は不定
        h8->ccr.set_z();
        h8->pc += 4;
        return 0;
    }

    int16_t quotient = dst_value / src_value;
    int16_t remainder = dst_value % src_value;

    // 上位16ビットに剰余、下位16ビットに商を入れる
    // H8 の仕様では剰余の符号は被除数と一致するとある
    // C/C++ の剰余はこの仕様に一致するため特に処置は必要ない
    dst.set((remainder << 16) | (int32_t)quotient);

    quotient_is_positive ? h8->ccr.clear_n() : h8->ccr.set_n();
    h8->ccr.clear_z();

    h8->pc += 4;

    return 0;
}