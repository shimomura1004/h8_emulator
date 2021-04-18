#include "mov_register_indirect.h"
#include "mov.h"

int  h8instructions::mov::register_indirect_b(H8300H* h8)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);
    const uint8_t b1_msb = b1 & 0x80;

    if (b1_msb) {
        // Rs,@ERd
        const uint8_t dst_register_index = (b1 & 0x70) >> 4;
        const uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        const Register8& src = h8->reg8[src_register_index];

        uint8_t value = src.get();
        uint32_t address = dst.get();
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else {
        // @ERs,Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        const Register32& src = h8->reg[src_register_index];
        Register8& dst = h8->reg8[dst_register_index];

        uint32_t address = src.get();
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    }

    h8->pc += 2;

    return 0;
}

int  h8instructions::mov::register_indirect_w(H8300H* h8)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);
    const uint8_t b1_msb = b1 & 0x80;

    if (b1_msb) {
        // Rs,@ERd
        const uint8_t dst_register_index = (b1 & 0x70) >> 4;
        const uint8_t src_register_index = (b1 & 0x0f);
        Register32& dst = h8->reg[dst_register_index];
        const Register16& src = h8->reg16[src_register_index];

        uint16_t value = src.get();
        uint32_t address = dst.get();
        h8->mcu.write16(address, value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
    } else {
        // @ERs,Rd
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        const Register32& src = h8->reg[src_register_index];
        Register16& dst = h8->reg16[dst_register_index];

        uint32_t address = src.get();
        uint16_t value = h8->mcu.read16(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
    }

    h8->pc += 2;

    return 0;
}

int h8instructions::mov::register_indirect_l(H8300H* h8)
{
    const uint8_t b3 = h8->fetch_instruction_byte(3);
    const uint8_t b3_msb = b3 & 0x80;

    if (b3_msb) {
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        uint8_t src_register_index = (b3 & 0x07);
        const Register32& dst = h8->reg[dst_register_index];
        const Register32& src = h8->reg[src_register_index];

        int32_t value = src.get();
        uint32_t address = dst.get();
        h8->mcu.write32(address, value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    } else {
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b3 & 0x07);
        const Register32& src = h8->reg[src_register_index];
        Register32& dst = h8->reg[dst_register_index];

        uint32_t address = src.get();
        int32_t value = h8->mcu.read32(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    }

    h8->pc += 4;

    return 0;
}
