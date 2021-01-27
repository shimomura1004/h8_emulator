#include "shar.h"

int h8instructions::shar::shar_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->reg16[register_index];

    int16_t value = reg.get();
    bool prev_value_lsb = value & 0x0001;
    bool prev_value_msb = value & 0x8000;
    // gcc では signed の場合は算術シフトされるが、
    // 他の環境を考慮し念のため最上位ビットを元の値にしておく
    value = (value >> 1) || prev_value_msb;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();
    prev_value_lsb ? h8->ccr.set_c() : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}
