#include "mov.h"

// todo: テンプレートでちゃんとやる
// todo: 種類ごとにファイルに分ける

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

static int register_indirect_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    if (b1_msb) {
        // Rs,@ERd
        uint8_t dst_register_index = (b1 & 0x70) >> 4;
        uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        Register32& src = h8->reg[src_register_index % 8];

        uint8_t value = (src_register_index < 8) ? src.get_rh() : src.get_rl();
        uint32_t address = dst.get_er();
        h8->memory.write_uint8(address, value);

        update_ccr(h8, value);
    } else {
        // @ERs,Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index % 8];

        uint32_t address = src.get_er();
        uint8_t value = h8->memory.read_uint8(address);
        if (dst_register_index < 8) {
            dst.set_rh(value);
        } else {
            dst.set_rl(value);
        }

        update_ccr(h8, value);
    }

    h8->pc += 2;

    return 0;
}

static int displacement_register_indirect16_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    if (b1_msb) {
        // Rs,@(d:16,ERd)
        uint8_t dst_register_index = (b1 & 0x70) >> 4;
        uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        Register32& src = h8->reg[src_register_index % 8];

        uint8_t value = (src_register_index < 8) ? src.get_rh() : src.get_rl();
        uint32_t address = dst.get_er() + disp;
        h8->memory.write_uint8(address, value);

        update_ccr(h8, value);
    } else {
        // @(d:16,ERs),Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index % 8];

        uint32_t address = src.get_er() + disp;
        uint8_t value = h8->memory.read_uint8(address);
        if (dst_register_index < 8) {
            dst.set_rh(value);
        } else {
            dst.set_rl(value);
        }

        update_ccr(h8, value);
    }

    h8->pc += 4;

    return 0;
}

static int displacement_register_indirect16_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    if (b1_msb) {
        // Rs,@(d:16,ERd)
        uint8_t dst_register_index = (b1 & 0x70) >> 4;
        uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        const Register32& src = h8->reg[src_register_index % 8];

        uint16_t value = (src_register_index < 8) ? src.get_r() : src.get_e();
        uint32_t address = dst.get_er() + disp;
        h8->memory.write_uint16(address, value);

        update_ccr(h8, value);
    } else {
        // @(d:16,ERs),Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index % 8];
        const Register32& src = h8->reg[src_register_index];

        uint32_t address = src.get_er() + disp;
        uint16_t value = h8->memory.read_uint16(address);
        (dst_register_index < 8) ? dst.set_r(value) : dst.set_e(value);

        update_ccr(h8, value);
    }

    h8->pc += 4;

    return 0;
}

static int displacement_register_indirect24_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b3h = (b3 & 0xf0) >> 4;

    uint8_t displacement[4];
    displacement[3] = 0;
    displacement[2] = h8->fetch_instruction_byte(5);
    displacement[1] = h8->fetch_instruction_byte(6);
    displacement[0] = h8->fetch_instruction_byte(7);
    int32_t disp = *(int32_t*)displacement;

    if (b3h == 0x02) {
        // @(d:24,ERs),Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b3 & 0x0f);
        Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index % 8];

        uint32_t address = src.get_er() + disp;
        uint8_t value = h8->memory.read_uint8(address);
        (dst_register_index < 8) ? dst.set_rh(value) : dst.set_rl(value);

        update_ccr(h8, value);
    } else if (b3h == 0x0a) {
        // // Rs,@(d:24,ERd)
        // uint8_t src_register_index = (b1 & 0x70) >> 4;
        // uint8_t dst_register_index = (b1 & 0x0f);
        // Register32& src = h8->reg[src_register_index];
        // Register32& dst = h8->reg[dst_register_index % 8];


        // uint32_t address = src.get_er() + disp;
        // uint8_t value = h8->memory.read_uint8(address);
        // if (dst_register_index < 8) {
        //     dst.set_rh(value);
        // } else {
        //     dst.set_rl(value);
        // }

        // update_ccr(h8, value);
        return -1;
    } else {
        fprintf(stderr, "Unknown mov.b format\n");
        return -1;
    }

    h8->pc += 8;

    return 0;
}

// static int register_indirect(H8300H* h8)
// {
//     return -1;
// }

// static int displacement_register_indirect16(H8300H* h8)
// {
//     return -1;
// }

static int displacement_register_indirect16_l_from_reg(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t dst_register_index = (b3 & 0x70) >> 4;
    uint8_t src_register_index = (b3 & 0x07);
    const Register32 dst = h8->reg[dst_register_index];
    const Register32 src = h8->reg[src_register_index];

    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(4);
    displacement[0] = h8->fetch_instruction_byte(5);
    int16_t disp = *(int16_t*)displacement;

    int32_t value = src.get_er();
    uint32_t address = dst.get_er() + disp;
    h8->memory.write_uint32(address, value);

    (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
    h8->ccr.clear_v();

    h8->pc += 6;

    return 0;
}

static int displacement_register_indirect24_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b5 = h8->fetch_instruction_byte(5);

    uint8_t displacement[4];
    displacement[3] = 0;
    displacement[2] = h8->fetch_instruction_byte(7);
    displacement[1] = h8->fetch_instruction_byte(8);
    displacement[0] = h8->fetch_instruction_byte(9);
    int32_t disp = *(int32_t*)displacement;

    if ((b3 & 0x80) == 0) {
        // @(d:24,ERs),ERd
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b5 & 0x07);
        Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index];
        uint32_t address = src.get_er() + disp;
        uint32_t value = h8->memory.read_uint32(address);
        dst.set_er(value);

        (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
        (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
        h8->ccr.clear_v();
    } else {
        // ERs,@(d:24,ERd)
        return -1;
    }

    h8->pc += 10;

    return 0;
}

static int post_increment_register_indirect_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    Register32& dst = h8->reg[dst_register_index % 8];

    uint8_t value = h8->pop_from_stack_b(src_register_index);
    if (dst_register_index < 8) {
        dst.set_rh(value);
    } else {
        dst.set_rl(value);
    }

    update_ccr(h8, value);
    h8->pc += 2;

    return 0;
}

static int post_increment_register_indirect_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_register_index = (b3 & 0x70) >> 4;
    uint8_t dst_register_index = (b3 & 0x07);
    Register32& dst = h8->reg[dst_register_index];

    uint32_t value = h8->pop_from_stack_l(src_register_index);
    dst.set_er(value);

    update_ccr(h8, value);
    h8->pc += 4;

    return 0;
}

static int pre_decrement_register_indirect_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_register_index = (b3 & 0x07);
    uint8_t dst_register_index = (b3 & 0x70) >> 4;
    Register32& src = h8->reg[src_register_index];

    h8->push_to_stack_l(src.get_er(), dst_register_index);
    update_ccr(h8, src.get_er());
    h8->pc += 4;

    return 0;
}

static int absolute_address_24_w_to_reg(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = b1 & 0x0f;
    Register32& dst = h8->reg[dst_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(3);
    absolute[1] = h8->fetch_instruction_byte(4);
    absolute[0] = h8->fetch_instruction_byte(5);
    int32_t abs = *(int32_t*)absolute;

    uint16_t value = h8->memory.read_uint16(abs);
    (dst_register_index < 8) ? dst.set_r(value) : dst.set_e(value);

    update_ccr(h8, value);
    h8->pc += 6;

    return 0;
}

static int absolute_address_24_w_from_reg(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = b1 & 0x0f;
    Register32& src = h8->reg[src_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(3);
    absolute[1] = h8->fetch_instruction_byte(4);
    absolute[0] = h8->fetch_instruction_byte(5);
    int32_t abs = *(int32_t*)absolute;

    uint16_t value = (src_register_index < 8) ? src.get_r() : src.get_e();
    h8->memory.write_uint16(abs, value);

    update_ccr(h8, value);
    h8->pc += 6;

    return 0;
}

static int absolute_address_24_l_to_reg(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t dst_register_index = b3 & 0x07;
    Register32& dst = h8->reg[dst_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(5);
    absolute[1] = h8->fetch_instruction_byte(6);
    absolute[0] = h8->fetch_instruction_byte(7);
    int32_t abs = *(int32_t*)absolute;

    int32_t value = h8->memory.read_uint32(abs);
    dst.set_er(value);

    update_ccr(h8, value);
    h8->pc += 8;

    return 0;
}

static int absolute_address_24_l_from_reg(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t src_register_index = b3 & 0x07;
    const Register32& src = h8->reg[src_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(5);
    absolute[1] = h8->fetch_instruction_byte(6);
    absolute[0] = h8->fetch_instruction_byte(7);
    int32_t abs = *(int32_t*)absolute;

    uint32_t value = src.get_er();
    h8->memory.write_uint32(abs, value);

    update_ccr(h8, value);
    h8->pc += 8;

    return 0;
}

static int immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    Register32& reg = h8->reg[register_index % 8];
    uint8_t value = h8->fetch_instruction_byte(1);

    if (register_index < 8) {
        reg.set_rh(value);
    } else {
        reg.set_rl(value);
    }

    update_ccr(h8, value);
    h8->pc += 2;

    return 0;
}

static int immediate_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    // ビッグエンディアンな即値をリトルエンディアンで読む
    uint8_t immediate[2];
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
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register32& reg = h8->reg[register_index];

    // ビッグエンディアンな即値をリトルエンディアンで読む
    uint8_t immediate[4];
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

static int register_direct_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    Register32& src = h8->reg[src_register_index % 8];
    Register32& dst = h8->reg[dst_register_index % 8];

    uint8_t value = (src_register_index < 8)
                        ? src.get_rh()
                        : src.get_rl();
    (dst_register_index < 8)
        ? dst.set_rh(value)
        : dst.set_rl(value);

    update_ccr(h8, value);
    h8->pc += 2;

    return 0;
}

static int register_direct_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    Register32& src = h8->reg[src_register_index % 8];
    Register32& dst = h8->reg[dst_register_index % 8];

    uint8_t value = (src_register_index < 8)
                        ? src.get_r()
                        : src.get_e();
    (dst_register_index < 8)
        ? dst.set_r(value)
        : dst.set_e(value);

    update_ccr(h8, value);
    h8->pc += 2;

    return 0;
}

static int register_direct_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x07);
    Register32& src = h8->reg[src_register_index];
    Register32& dst = h8->reg[dst_register_index];

    dst.set_er(src.get_er());
    update_ccr(h8, src.get_er());
    h8->pc += 2;

    return 0;
}

int h8instructions::mov::mov(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    switch (b0) {
    case 0x68: return register_indirect_b(h8);
    case 0x6b: {
        uint8_t b1 = h8->fetch_instruction_byte(1);
        uint8_t b1h = (b1 & 0xf0) >> 4;
        switch (b1h) {
            case 0x00: return -1;
            case 0x02: return absolute_address_24_w_to_reg(h8);
            case 0x08: return -1;
            case 0x0a: return absolute_address_24_w_from_reg(h8);
            default:   return -1;
        }
    }
    case 0x6c: {
        uint8_t b1 = h8->fetch_instruction_byte(1);
        if ((b1 & 0x80) == 0) {
            return post_increment_register_indirect_b(h8);
        } else {
            return -1;
        }
    }
    case 0x6e: return displacement_register_indirect16_b(h8);
    case 0x6f: return displacement_register_indirect16_w(h8);
    case 0xf0: case 0xf1: case 0xf2: case 0xf3:
    case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    case 0xf8: case 0xf9: case 0xfa: case 0xfb:
    case 0xfc: case 0xfd: case 0xfe: case 0xff:
        return immediate_b(h8);
    case 0x01: {
        uint8_t b2 = h8->fetch_instruction_byte(2);
        switch (b2) {
        //case 0x69: return register_indirect(h8);
        case 0x78: return displacement_register_indirect24_l(h8);
        case 0x6b: {
            uint8_t b3 = h8->fetch_instruction_byte(3);
            switch ((b3 & 0xf0) >> 4) {
            case 0x0a: return absolute_address_24_l_from_reg(h8);
            default:   return absolute_address_24_l_to_reg(h8);
            }
        }
        case 0x6d: {
            uint8_t b3 = h8->fetch_instruction_byte(3);
            if ((b3 & 0x80) == 0) {
                return post_increment_register_indirect_l(h8);
            } else {
                return pre_decrement_register_indirect_l(h8);
            }
        }
        case 0x6f: {
            uint8_t b3 = h8->fetch_instruction_byte(3);
            if (b3 & 0x80) {
                return displacement_register_indirect16_l_from_reg(h8);
            } else {
                return -1;
            }
        }
        default: return -1;
        }
    }
    case 0x78: return displacement_register_indirect24_b(h8);
    case 0x79: return immediate_w(h8);
    case 0x7a: return immediate_l(h8);
    case 0x0c: return register_direct_b(h8);
    case 0x0d: return register_direct_w(h8);
    case 0x0f: return register_direct_l(h8);
    default:
        fprintf(stderr, "Unknown error in %s\n", __FILE__);
        return -1;
    }
}
