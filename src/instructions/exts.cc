#include "exts.h"

int h8instructions::exts::exts_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x0f);
    Register16& reg = h8->cpu.reg16(register_index);

    // 下位8ビットの MSB (符号ビット)を抜き出す
    bool msb = reg.get() & 0x0080;

    if (msb) {
        // 最上位ビットが1なので負数、上位1バイトを1で埋める
        reg.set(reg.get() | 0xff00);
        h8->cpu.ccr().set_n();
    } else {
        // 最上位ビットが0なので正数、上位1バイトを 0 で埋める
        reg.set(reg.get() & 0x00ff);
        h8->cpu.ccr().clear_n();
    }

    if (reg.get() == 0) {
        h8->cpu.ccr().set_z();        
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::exts::exts_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x07);
    Register32& reg = h8->cpu.reg32(register_index);

    bool msb = reg.get() & 0x00008000;

    if (msb) {
        // 最上位ビットが負
        reg.set(reg.get() | 0xffff0000);
        h8->cpu.ccr().set_n();
    } else {
        // 最上位ビットが正
        reg.set(reg.get() & 0x0000ffff);
        h8->cpu.ccr().clear_n();
    }

    if (reg.get() == 0) {
        h8->cpu.ccr().set_z();        
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}
