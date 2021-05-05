#include "rotl.h"

int h8instructions::rotl::rotl_b(H8Board* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char reg_index = (b1 & 0x0f);
    Register8& reg = h8->cpu.reg8(reg_index);

    uint8_t result_value = reg.get();
    bool msb = result_value & 0x80;
    // レジスタを左シフト
    result_value = result_value << 1;
    // 0ビット目に MSB の値を入れる
    result_value |= (uint8_t)msb;

    reg.set(result_value);

    (result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    // CCR.C にキャリーアウトされた値を入れる
    msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}
