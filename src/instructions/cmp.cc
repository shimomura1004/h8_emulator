#include "cmp.h"
#include "sub.h"

int h8instructions::cmp::cmp_immediate_b(H8Board *h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    const Register8 reg = h8->cpu.reg8(register_index);

    int8_t src_value = h8->fetch_instruction_byte(1);
    int8_t dst_value = reg.get();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_b(H8Board *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = b1 & 0x0f;
    const Register8& src = h8->cpu.reg8(src_register_index);
    const Register8& dst = h8->cpu.reg8(dst_register_index);

    int8_t src_value = src.get();
    int8_t dst_value = dst.get();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::cmp::cmp_immediate_w(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x0f;
    const Register16& reg = h8->cpu.reg16(register_index);

    uint8_t immediate[2];
    immediate[1] = h8->fetch_instruction_byte(2);
    immediate[0] = h8->fetch_instruction_byte(3);
    int16_t src_value = *(int16_t*)immediate;

    int16_t dst_value = reg.get();
    int16_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 4;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_w(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = b1 & 0x0f;
    const Register16& src = h8->cpu.reg16(src_register_index);
    const Register16& dst = h8->cpu.reg16(dst_register_index);

    int16_t src_value = src.get();
    int16_t dst_value = dst.get();
    int16_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<16, int16_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}

int h8instructions::cmp::cmp_immediate_l(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = b1 & 0x07;
    const Register32& reg = h8->cpu.reg32(register_index);

    uint8_t immediate[4];
    immediate[3] = h8->fetch_instruction_byte(2);
    immediate[2] = h8->fetch_instruction_byte(3);
    immediate[1] = h8->fetch_instruction_byte(4);
    immediate[0] = h8->fetch_instruction_byte(5);
    int32_t src_value = *(int32_t*)immediate;

    int32_t dst_value = reg.get();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 6;

    return 0;
}

int h8instructions::cmp::cmp_register_direct_l(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0x70) >> 4;
    uint8_t dst_register_index = b1 & 0x07;
    const Register32& src = h8->cpu.reg32(src_register_index);
    const Register32& dst = h8->cpu.reg32(dst_register_index);

    int32_t src_value = src.get();
    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<32, int32_t>(h8, src_value, dst_value, result_value);
    h8->cpu.pc() += 2;

    return 0;
}
