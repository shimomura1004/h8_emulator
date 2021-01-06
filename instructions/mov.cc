#include "mov.h"

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

static int register_indirect(H8300H* h8)
{
    return -1;
}

static int displacement_register_indirect16(H8300H* h8)
{
    return -1;
}

static int displacement_register_indirect24(H8300H* h8)
{
    unsigned char b3 = h8->fetch_instruction_byte(3);
    unsigned char b5 = h8->fetch_instruction_byte(5);
    unsigned char b7 = h8->fetch_instruction_byte(7);
    unsigned char b8 = h8->fetch_instruction_byte(8);
    unsigned char b9 = h8->fetch_instruction_byte(9);

    unsigned char displacement[4];
    displacement[3] = 0;
    displacement[2] = h8->fetch_instruction_byte(7);
    displacement[1] = h8->fetch_instruction_byte(8);
    displacement[0] = h8->fetch_instruction_byte(9);
    int32_t disp = *(int32_t*)displacement;

    if ((b3 & 0x80) == 0) {
        // @(d:24,ERs),ERd
        unsigned char src_register_index = (b3 & 0x70) >> 4;
        unsigned char dst_register_index = (b5 & 0x07);
        Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index];
        uint32_t address = src.get_er() + *(uint32_t*)displacement;
        uint32_t value = h8->memory.read_uint32(address);
        dst.set_er(value);

        (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
        (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
        h8->ccr.clear_v();
    } else {
        // ERs,@(d:24,ERd)
        unsigned char dst_register_index = (b3 & 0x70) >> 4;
        unsigned char src_register_index = (b5 & 0x07);

        return -1;
    }

    h8->pc += 10;

    return 0;
}

static int post_increment_register_indirect(H8300H* h8)
{
    return -1;
}

static int pre_decrement_register_indirect(H8300H* h8)
{
    unsigned char instruction_byte_3 = h8->fetch_instruction_byte(3);
    unsigned char src_register_index = (instruction_byte_3 & 0x07);
    unsigned char dst_register_index = (instruction_byte_3 & 0x70) >> 4;
    Register32& src = h8->reg[src_register_index];

    h8->push_to_stack_l(src.get_er(), dst_register_index);
    update_ccr(h8, src.get_er());
    h8->pc += 4;

    return 0;
}

static int absolute_address(H8300H* h8)
{
    return -1;
}

static int immediate_w(H8300H* h8)
{
    unsigned char instruction_byte_1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = instruction_byte_1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    // ビッグエンディアンな即値をリトルエンディアンで読む
    unsigned char immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    int16_t imm = *(int16_t*)immediate;

    reg.set_r(imm);
    update_ccr(h8, imm);
    h8->pc += 4;

    return 0;
}

static int immediate_l(H8300H* h8)
{
    unsigned char instruction_byte_1 = h8->fetch_instruction_byte(1);
    unsigned char register_index = instruction_byte_1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    // ビッグエンディアンな即値をリトルエンディアンで読む
    unsigned char immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t imm = *(int32_t*)immediate;

    reg.set_er(imm);
    update_ccr(h8, imm);
    h8->pc += 6;

    return 0;
}

static int register_direct(H8300H* h8)
{
    unsigned char instruction_byte_1 = h8->fetch_instruction_byte(1);
    unsigned char src_register_index = (instruction_byte_1 & 0x70) >> 4;
    unsigned char dst_register_index = (instruction_byte_1 & 0x07);
    Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    dst.set_er(src.get_er());
    update_ccr(h8, src.get_er());
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::mov(H8300H* h8)
{
    unsigned char b0 = h8->fetch_instruction_byte(0);

    switch (b0) {
    case 0x01: {
        unsigned char b2 = h8->fetch_instruction_byte(2);
        switch (b2) {
        case 0x69: return register_indirect(h8);
        case 0x6f: return displacement_register_indirect16(h8);
        case 0x78: return displacement_register_indirect24(h8);
        case 0x6d: {
            unsigned char b3 = h8->fetch_instruction_byte(3);
            return ((b3 & 0x80) == 0)
                ? post_increment_register_indirect(h8)
                : pre_decrement_register_indirect(h8);
        }
        case 0x6b: return absolute_address(h8);
        }
    }
    case 0x79: return immediate_w(h8);
    case 0x7a: return immediate_l(h8);
    case 0x0f: return register_direct(h8);
    default:
        fprintf(stderr, "Unknown error in %s\n", __FILE__);
        return -1;
    }
}
