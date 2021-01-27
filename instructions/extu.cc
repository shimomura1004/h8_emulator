#include "extu.h"

int h8instructions::extu::extu_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x0f);
    Register16& reg = h8->reg16[register_index];

    uint8_t value = reg.get() & 0x00ff;

    // 上位1バイトを 0 で埋める
    reg.set(0x00ff & value);

    h8->ccr.clear_n();

    if (reg.get() == 0) {
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
