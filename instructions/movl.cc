#include "movl.h"

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

    update_ccr(h8300h, imm);

    h8300h->pc += 6;

    return 0;
}

int h8instructions::movl::regs::execute(H8300H* h8300h)
{
    unsigned char instruction_byte_2 = h8300h->fetch_instruction_byte(1);
    unsigned char src_register_index = (instruction_byte_2 & 0x70) >> 4;
    unsigned char dst_register_index = (instruction_byte_2 & 0x07);
    Register32& src = h8300h->reg[src_register_index];
    Register32& dst = h8300h->reg[dst_register_index];

    dst.set_er(src.get_er());

    update_ccr(h8300h, src.get_er());
    h8300h->pc += 2;

    return 0;
}

int h8instructions::movl::execute(H8300H* h8300h)
{
    unsigned char instruction_byte_3 = h8300h->fetch_instruction_byte(2);
    switch (instruction_byte_3) {
    case 0x6d: {
        unsigned char instruction_byte_4 = h8300h->fetch_instruction_byte(3);
        unsigned char src_register_index = (instruction_byte_4 & 0x07);
        unsigned char dst_register_index = (instruction_byte_4 & 0x70) >> 4;
        Register32& src = h8300h->reg[src_register_index];
        Register32& dst = h8300h->reg[dst_register_index];

        if (instruction_byte_4 & 0x80) {
            // プリデクリメントレジスタ間接
            h8300h->push_to_stack_l(src.get_er(), dst_register_index);
        }

        update_ccr(h8300h, src.get_er());
        h8300h->pc += 4;
        break;
    }
    default:
        fprintf(stderr, "Unknown jsr\n");
        return -1;
    }

    return 0;
}