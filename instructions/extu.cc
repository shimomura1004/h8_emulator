#include "extu.h"

int h8instructions::extu::extu_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x0f);
    Register32& reg = h8->reg[register_index % 8];

    uint8_t value = (register_index < 8) ? reg.get_r() : reg.get_e();

    // 上位1バイトを 0 で埋める
    (register_index < 8) ? reg.set_r(0x00ff & value) : reg.set_r(0x00ff & value);

    h8->ccr.clear_n();

    if (reg.get_r() == 0) {
        h8->ccr.set_z();        
    } else {
        h8->ccr.clear_z();
    }

    h8->ccr.clear_v();

    h8->pc += 2;

    return 0;
}

int h8instructions::extu::extu_l(H8300H* h8)
{
    return -1;
}
