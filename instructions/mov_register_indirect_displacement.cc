#include "mov_register_indirect_displacement.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement16_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    if (b1_msb) {
        // Rs,@(d:16,ERd)
        const uint8_t dst_register_index = (b1 & 0x70) >> 4;
        const uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        const Register8& src = h8->reg8[src_register_index];

        uint8_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else {
        // @(d:16,ERs),Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        const Register32& src = h8->reg[src_register_index];
        Register8& dst = h8->reg8[dst_register_index];

        uint32_t address = src.get() + disp;
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    }

    h8->pc += 4;

    return 0;
}

int register_indirect_with_displacement16_w(H8300H* h8)
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
        const Register16& src = h8->reg16[src_register_index];

        uint16_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write16(address, value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
    } else {
        // @(d:16,ERs),Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register16& dst = h8->reg16[dst_register_index];
        const Register32& src = h8->reg[src_register_index];

        uint32_t address = src.get() + disp;
        uint16_t value = h8->mcu.read16(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
    }

    h8->pc += 4;

    return 0;
}

int register_indirect_with_displacement16_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b3_msb = b3 & 0x80;

    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(4);
    displacement[0] = h8->fetch_instruction_byte(5);
    int16_t disp = *(int16_t*)displacement;

    if (b3_msb) {
        uint8_t b3 = h8->fetch_instruction_byte(3);
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        uint8_t src_register_index = (b3 & 0x07);
        const Register32& dst = h8->reg[dst_register_index];
        const Register32& src = h8->reg[src_register_index];

        int32_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write32(address, value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    } else {
        uint8_t b3 = h8->fetch_instruction_byte(3);
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b3 & 0x07);
        const Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index];

        uint32_t address = src.get() + disp;
        int32_t value = h8->mcu.read32(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    }

    h8->pc += 6;

    return 0;
}


int register_indirect_with_displacement24_b(H8300H* h8)
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
        const Register32& src = h8->reg[src_register_index];
        Register8& dst = h8->reg8[dst_register_index];

        uint32_t address = src.get() + disp;
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else if (b3h == 0x0a) {
        // Rs,@(d:24,ERd)
        uint8_t dst_register_index = (b1 & 0x70) >> 4;
        uint8_t src_register_index = (b3 & 0x0f);
        const Register32& dst = h8->reg[dst_register_index];
        const Register8& src = h8->reg8[src_register_index];

        uint8_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else {
        fprintf(stderr, "Unknown mov.b format\n");
        return -1;
    }

    h8->pc += 8;

    return 0;
}


int register_indirect_with_displacement24_l(H8300H* h8)
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
        uint32_t address = src.get() + disp;
        uint32_t value = h8->mcu.read32(address);
        dst.set(value);

        (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
        (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
        h8->ccr.clear_v();
    } else {
        // ERs,@(d:24,ERd)
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        uint8_t src_register_index = (b5 & 0x07);
        const Register32& dst = h8->reg[dst_register_index];
        const Register32& src = h8->reg[src_register_index];

        uint32_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write32(address, value);

        (value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
        (value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();
        h8->ccr.clear_v();
    }

    h8->pc += 10;

    return 0;
}

int register_indirect_with_increment_decrement_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    if ((b1 & 0x80) == 0) {
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register8& dst = h8->reg8[dst_register_index];

        uint8_t value = h8->pop_from_stack_b(src_register_index);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
        h8->pc += 2;
    } else {
        return -1;
    }

    return 0;
}

int register_indirect_with_increment_decrement_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    if ((b3 & 0x80) == 0) {
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b3 & 0x07);
        Register32& dst = h8->reg[dst_register_index];

        uint32_t value = h8->pop_from_stack_l(src_register_index);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    } else {
        uint8_t src_register_index = (b3 & 0x07);
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        Register32& src = h8->reg[src_register_index];

        h8->push_to_stack_l(src.get(), dst_register_index);
        h8instructions::mov::update_ccr<int32_t>(h8, src.get());
    }

    h8->pc += 4;

    return 0;
}

}
}