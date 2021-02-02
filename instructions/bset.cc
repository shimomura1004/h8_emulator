#include "bset.h"

int h8instructions::bset::bset_register_direct_imm(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t imm = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    Register8& dst = h8->reg8[dst_register_index];

    uint8_t mask = (1 << imm);
    uint8_t value = dst.get();
    value |= mask;
    dst.set(value);

    h8->pc += 2;

    return 0;
}

int h8instructions::bset::bset_register_indirect(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = (b1 & 0x70) >> 4;
    const Register32& reg = h8->reg[reg_index];

    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t imm = (b3 & 0x70) >> 4;

    uint32_t address = reg.get();
    uint8_t value = h8->mcu.read8(address);

    uint8_t mask = (1 << imm);
    value |= mask;
    h8->mcu.write8(address, value);

    h8->pc += 4;

    return 0;
}
