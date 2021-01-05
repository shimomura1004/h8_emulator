#include "movl.h"

int h8instructions::movl::immediate::execute(H8300H* h8300h)
{
    unsigned char instruction_byte_2 = h8300h->fetch_instruction_byte(1);
    unsigned char register_index = instruction_byte_2 & 0x0f;
    Register32& reg = h8300h->reg[register_index];

    // ビッグエンディアンな即値をリトルエンディアンで読む
    unsigned char immediate[4];
    immediate[3] = h8300h->fetch_instruction_byte(2);
    immediate[2] = h8300h->fetch_instruction_byte(3);
    immediate[1] = h8300h->fetch_instruction_byte(4);
    immediate[0] = h8300h->fetch_instruction_byte(5);
    int32_t imm = *(int32_t*)immediate;
    reg.set_er(imm);

    // update ccr
    CCR& ccr = h8300h->ccr;
    if (imm < 0) {
        ccr.set_n();
    } else {
        ccr.clear_n();
    }

    if (imm == 0) {
        ccr.set_z();
    } else {
        ccr.clear_z();
    }

    ccr.clear_v();

    h8300h->pc += 6;

    return 0;
}
