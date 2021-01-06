#include "movw.h"

// todo: 共通の処理なのでくくりだす
static void update_ccr(H8300H* h8300h, int32_t value) {
    CCR& ccr = h8300h->ccr;
    if (value < 0) {
        ccr.set_n();
    } else {
        ccr.clear_n();
    }

    if (value == 0) {
        ccr.set_z();
    } else {
        ccr.clear_z();
    }

    ccr.clear_v();
}

int h8instructions::movw::immediate::execute(H8300H* h8300h)
{
    unsigned char instruction_byte_2 = h8300h->fetch_instruction_byte(1);
    unsigned char register_index = instruction_byte_2 & 0x0f;
    Register32& reg = h8300h->reg[register_index];

    switch ((instruction_byte_2 & 0xf0) >> 4) {
    case 0x00: {
        // 即値コピー
        unsigned char immediate[2];
        immediate[1] = h8300h->fetch_instruction_byte(2);
        immediate[0] = h8300h->fetch_instruction_byte(3);
        int16_t imm = *(int16_t*)immediate;
        reg.set_r(imm);
        update_ccr(h8300h, imm);
        h8300h->pc += 4;
        break;
    }
    default:
        break;
    }

    return 0;
}