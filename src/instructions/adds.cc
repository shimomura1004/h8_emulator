#include "adds.h"

int h8instructions::adds::adds(H8Board* h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = b1 & 0x0f;
    Register32& reg = h8->cpu.reg32(register_index);

    switch (b1 & 0xf0) {
    case 0x00:
        reg.set(reg.get() + 1);
        break;
    case 0x80:
        reg.set(reg.get() + 2);
        break;
    case 0x90:
        reg.set(reg.get() + 4);
        break;
    default:
        return -1;
    }

    h8->cpu.pc() += 2;

    return 0;
}
