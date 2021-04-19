#include "mov_immediate.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

int immediate_b(H8300H* h8)
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

int immediate_w(H8300H* h8)
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

int immediate_l(H8300H* h8)
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

}
}
