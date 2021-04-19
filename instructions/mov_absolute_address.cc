#include "mov_absolute_address.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

// todo: パーサは同じだから switch で分岐すれば十分？
int absolute_address_24_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;

    switch (b1h) {
    case 0x00: return -1;
    case 0x02: return -1;
    case 0x08: return -1;
    case 0x0a: {
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
    default:
        return -1;
    }
}

int absolute_address_24_w(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;
    switch (b1h) {
    case 0x00: return -1;
    case 0x02: {
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
    case 0x08: return -1;
    case 0x0a: {
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
    default:
        return -1;
    }
}

int absolute_address_24_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    switch ((b3 & 0xf0) >> 4) {
    case 0x02: {
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
    case 0x0a: {
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
    default:
        return -1;
    }
}

}
}
