#include "adds.h"

// 32ビットレジスタ ERd の内容に 1/2/4 を加算する命令

int h8instructions::adds::execute(H8300H* h8300h)
{
    unsigned char instruction_byte_2 = h8300h->fetch_instruction_byte(1);
    unsigned char register_index = instruction_byte_2 & 0x0f;
    Register32& reg = h8300h->reg[register_index];

    switch (instruction_byte_2 & 0xf0) {
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

    h8300h->pc += 2;

    return 0;
}
