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
