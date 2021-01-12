#include "cmp.h"
#include "sub.h"

int h8instructions::cmp::cmp_immediate_b(H8300H *h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    const Register32& reg = h8->reg[register_index % 8];

    int8_t src_value = h8->fetch_instruction_byte(1);
    int8_t dst_value = (register_index < 8) ? reg.get_rh() : reg.get_rl();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_b(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = b1 & 0x0f;
    const Register32& src = h8->reg[src_register_index % 8];
    const Register32& dst = h8->reg[dst_register_index % 8];

    int8_t src_value = (src_register_index < 8) ? src.get_rh() : src.get_rl();
    int8_t dst_value = (dst_register_index < 8) ? dst.get_rh() : dst.get_rl();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = b1 & 0x0f;
    const Register32& src = h8->reg[src_register_index % 8];
    const Register32& dst = h8->reg[dst_register_index % 8];

    int16_t src_value = (src_register_index < 8) ? src.get_r() : src.get_e();
    int16_t dst_value = (dst_register_index < 8) ? dst.get_r() : dst.get_e();
    int16_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

int h8instructions::cmp::cmp_immediate_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    const Register32& reg = h8->reg[register_index];

    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t src_value = *(int32_t*)immediate;

    int32_t dst_value = reg.get_er();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->pc += 6;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_l(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = b1 & 0x07;
    const Register32& src = h8->reg[src_register_index];
    const Register32& dst = h8->reg[dst_register_index];

    int32_t src_value = src.get_er();
    int32_t dst_value = dst.get_er();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}
