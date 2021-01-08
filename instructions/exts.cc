#include "exts.h"

int h8instructions::exts::exts_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x0f);
    Register32& reg = h8->reg[register_index % 8];

    uint8_t value = (register_index < 8) ? reg.get_r() : reg.get_e();
    // 下位8ビットの MSB (符号ビット)を抜き出す
    bool lower_sign_bit = value & 0x0080;

    if (lower_sign_bit) {
        // 最上位ビットが1なので負数、上位1バイトを1で埋める
        (register_index < 8) ? reg.set_r(0xff00 | value) : reg.set_e(0xff00 | value);
        h8->ccr.set_n();
    } else {
        // 上位1バイトを 0 で埋める
        (register_index < 8) ? reg.set_r(0x00ff & value) : reg.set_r(0x00ff & value);
        h8->ccr.clear_n();
    }

    if (reg.get_r() == 0) {
        h8->ccr.set_z();        
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}

int h8instructions::exts::exts_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x07);
    Register32& reg = h8->reg[register_index];

    uint16_t r = reg.get_r();
    uint16_t r_msb = r >> 15;

    if (r_msb) {
        // 最上位ビットが1なので負数
        reg.set_e(0xffff);
        h8->ccr.set_n();
    } else {
        reg.set_e(0x0000);
        h8->ccr.clear_n();
    }

    if (reg.get_er() == 0) {
        h8->ccr.set_z();        
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}
