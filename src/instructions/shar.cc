#include "shar.h"

// TODO: v　をクリアするところが違うが、だいたい同じ
template<class T>
static void update_ccr(H8Board* h8, T value, bool lsb)
{
    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    lsb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();
}

int h8instructions::shar::shar_w(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->cpu.reg16(register_index);

    int16_t value = reg.get();
    bool prev_value_lsb = value & 0x0001;
    bool prev_value_msb = value & 0x8000;
    // gcc では signed の場合は算術シフトされるが、
    // 他の環境を考慮し念のため最上位ビットを元の値にしておく
    value = (value >> 1) | (prev_value_msb ? 0x8000 : 0);
    reg.set(value);

    ::update_ccr<int16_t>(h8, value, prev_value_lsb);

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::shar::shar_l(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    Register32& reg = h8->cpu.reg32(register_index);

    int32_t value = reg.get();
    bool prev_value_lsb = value & 0x00000001;
    bool prev_value_msb = value & 0x80000000;
    // gcc では signed の場合は算術シフトされるが、
    // 他の環境を考慮し念のため最上位ビットを元の値にしておく
    value = (value >> 1) | (prev_value_msb ? 0x80000000 : 0);
    reg.set(value);

    ::update_ccr<int32_t>(h8, value, prev_value_lsb);

    h8->cpu.pc() += 2;

    return 0;
}
