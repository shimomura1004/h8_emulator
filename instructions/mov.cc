#include "mov.h"

// todo: 種類ごとにファイルに分ける
// todo: displacement などの置き換え

static int absolute_address_24_b_from_reg(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = b1 & 0x0f;
    const Register8& src = h8->reg8[src_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(3);
    absolute[1] = h8->fetch_instruction_byte(4);
    absolute[0] = h8->fetch_instruction_byte(5);
    int32_t abs = *(int32_t*)absolute;

    uint8_t value = src.get();
    h8->mcu.write8(abs, value);

    h8instructions::mov::update_ccr<int8_t>(h8, (int8_t)value);
    h8->pc += 6;

    return 0;
}

static int absolute_address_24_w_to_reg(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t dst_register_index = b1 & 0x0f;
    Register16& dst = h8->reg16[dst_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(3);
    absolute[1] = h8->fetch_instruction_byte(4);
    absolute[0] = h8->fetch_instruction_byte(5);
    int32_t abs = *(int32_t*)absolute;

    uint16_t value = h8->mcu.read16(abs);
    dst.set(value);

    h8instructions::mov::update_ccr<int16_t>(h8, value);
    h8->pc += 6;

    return 0;
}

static int absolute_address_24_w_from_reg(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = b1 & 0x0f;
    const Register16& src = h8->reg16[src_register_index];

    uint8_t absolute[4];
    absolute[3] = 0;
    absolute[2] = h8->fetch_instruction_byte(3);
    absolute[1] = h8->fetch_instruction_byte(4);
    absolute[0] = h8->fetch_instruction_byte(5);
    int32_t abs = *(int32_t*)absolute;

    uint16_t value = src.get();
    h8->mcu.write16(abs, value);

    h8instructions::mov::update_ccr<int16_t>(h8, value);
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

    int32_t value = h8->mcu.read32(abs);
    dst.set(value);

    h8instructions::mov::update_ccr<int32_t>(h8, value);
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

    uint32_t value = src.get();
    h8->mcu.write32(abs, value);

    h8instructions::mov::update_ccr<int32_t>(h8, value);
    h8->pc += 8;

    return 0;
}

static int immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    Register8& reg = h8->reg8[register_index];
    uint8_t value = h8->fetch_instruction_byte(1);

    reg.set(value);

    h8instructions::mov::update_ccr<int8_t>(h8, value);
    h8->pc += 2;

    return 0;
}

static int immediate_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    Register16& reg = h8->reg16[register_index];

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    int16_t imm = *(int16_t*)immediate;

    reg.set(imm);
    h8instructions::mov::update_ccr<int16_t>(h8, imm);
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

    reg.set(imm);
    h8instructions::mov::update_ccr<int32_t>(h8, imm);
    h8->pc += 6;

    return 0;
}

int h8instructions::mov::mov(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    switch (b0) {
    case 0x01: {
        uint8_t b2 = h8->fetch_instruction_byte(2);
        switch (b2) {
        case 0x69: return -1;
        case 0x6b: {
            uint8_t b3 = h8->fetch_instruction_byte(3);
            switch ((b3 & 0xf0) >> 4) {
            case 0x0a: return absolute_address_24_l_from_reg(h8);
            default:   return absolute_address_24_l_to_reg(h8);
            }
        }
        case 0x6d: return -1;
        case 0x6f: return -1;
        case 0x78: return -1;
        default: return -1;
        }
    }
    case 0x0c: return -1;
    case 0x0d: return -1;
    case 0x0f: return -1;
    case 0x68: return -1;
    case 0x69: return -1;
    case 0x6a: {
        uint8_t b1 = h8->fetch_instruction_byte(1);
        uint8_t b1h = (b1 & 0xf0) >> 4;
        switch (b1h) {
        case 0x00: return -1;
        case 0x02: return -1;
        case 0x08: return -1;
        case 0x0a: return absolute_address_24_b_from_reg(h8);
        }
    }
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
    case 0x6c: return -1;
    case 0x6e: return -1;
    case 0x6f: return -1;
    case 0x78: return -1;
    case 0x79: return immediate_w(h8);
    case 0x7a: return immediate_l(h8);
    case 0xf0: case 0xf1: case 0xf2: case 0xf3:
    case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    case 0xf8: case 0xf9: case 0xfa: case 0xfb:
    case 0xfc: case 0xfd: case 0xfe: case 0xff:
        return immediate_b(h8);
    default:
        fprintf(stderr, "Unknown error in %s\n", __FILE__);
        return -1;
    }
}
