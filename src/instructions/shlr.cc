#include "shlr.h"

int h8instructions::shlr::shlr_b(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register8& reg = h8->reg8[register_index];

    uint8_t value = reg.get();
    bool prev_value_lsb = value & 0x01;

    // gcc では unsigned の場合は論理シフトされるので不要だが、
    // 他の環境を考慮し念のため最上位ビットを 0 にしておく
    value = (value >> 1) & 0x7f;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();
    prev_value_lsb ? h8->ccr.set_c() : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}

int h8instructions::shlr::shlr_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->reg16[register_index];

    uint16_t value = reg.get();
    bool prev_value_lsb = value & 0x01;

    // gcc では unsigned の場合は論理シフトされるので不要だが、
    // 他の環境を考慮し念のため最上位ビットを 0 にしておく
    value = (value >> 1) & 0x7fff;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();
    prev_value_lsb ? h8->ccr.set_c() : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}

int h8instructions::shlr::shlr_l(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    Register32& reg = h8->reg[register_index];

    uint32_t value = reg.get();
    bool prev_value_lsb = value & 0x01;

    // gcc では unsigned の場合は論理シフトされるので不要だが、
    // 他の環境を考慮し念のため最上位ビットを 0 にしておく
    value = (value >> 1) & 0x7fffffff;
    reg.set(value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();
    prev_value_lsb ? h8->ccr.set_c() : h8->ccr.clear_c();

    h8->pc += 2;

    return 0;
}
