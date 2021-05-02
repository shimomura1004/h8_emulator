#include "extu.h"

// todo: ccr の更新部分を共通化

int h8instructions::extu::extu_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x0f);
    Register16& reg = h8->cpu.reg16(register_index);

    uint8_t value = reg.get() & 0x00ff;

    // 上位1バイトを 0 で埋める
    reg.set(0x00ff & (uint16_t)value);

    h8->cpu.ccr().clear_n();

    if (reg.get() == 0) {
        h8->cpu.ccr().set_z();        
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::extu::extu_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x07);
    Register32& reg = h8->cpu.reg32(register_index);

    uint16_t value = reg.get() & 0x0000ffff;

    // 上位2バイトを 0 で埋める
    reg.set(0x0000ffff & (uint32_t)value);

    h8->cpu.ccr().clear_n();

    if (reg.get() == 0) {
        h8->cpu.ccr().set_z();        
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();

    h8->cpu.pc() += 2;

    return 0;
}
