#include "subs.h"

int h8instructions::subs::subs(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    switch ((b1 & 0xf0) >> 4)
    {
    case 0x00:
        reg.set_er(reg.get_er() - 1);
        break;
    case 0x08:
        reg.set_er(reg.get_er() - 2);
        break;
    case 0x09:
        reg.set_er(reg.get_er() - 4);
        break;
    default:
        return -1;
    }

    h8->pc += 2;

    return 0;
}