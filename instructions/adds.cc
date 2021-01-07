#include "adds.h"

int h8instructions::adds::adds(H8300H* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = b1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    switch (b1 & 0xf0) {
    case 0x00:
        reg.set_er(reg.get_er() + 1);
        break;
    case 0x80:
        reg.set_er(reg.get_er() + 2);
        break;
    case 0x90:
        reg.set_er(reg.get_er() + 4);
        break;
    default:
        return -1;
    }

    h8->pc += 2;

    return 0;
}
