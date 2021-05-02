#include "rotxl.h"

int h8instructions::rotxl::rotxl_l(H8300H* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char reg_index = (b1 & 0x07);
    Register32& reg = h8->cpu.reg32(reg_index);

    uint32_t result_value = reg.get();
    bool msb = result_value & 0x80000000;
    // レジスタを左シフト
    result_value = result_value << 1;
    // 0ビット目に CCR.C の値を入れる
    result_value |= (uint32_t)h8->cpu.ccr().c();

    reg.set(result_value);

    (result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    // CCR.C にキャリーアウトされた値を入れる
    msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}
