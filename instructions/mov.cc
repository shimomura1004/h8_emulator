#include "mov.h"

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
        case 0x6b: return -1;
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
    case 0x6a: return -1;
    case 0x6b: return -1;
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
