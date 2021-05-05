#include "subs.h"

int h8instructions::subs::subs(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register32& reg = h8->cpu.reg32(register_index);

    switch ((b1 & 0xf0) >> 4)
    {
    case 0x00:
        reg.set(reg.get() - 1);
        break;
    case 0x08:
        reg.set(reg.get() - 2);
        break;
    case 0x09:
        reg.set(reg.get() - 4);
        break;
    default:
        return -1;
    }

    h8->cpu.pc() += 2;

    return 0;
}