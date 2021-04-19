#include "mov_absolute_address.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

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


void absolute_address_24_b_parse(H8300H* h8, Instruction* instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;

    switch (b1h) {
    case 0x00: return;
    case 0x02: return;
    case 0x08: return;
    case 0x0a: {
        uint8_t abs[4];
        abs[3] = 0;
        abs[2] = h8->fetch_instruction_byte(3);
        abs[1] = h8->fetch_instruction_byte(4);
        abs[0] = h8->fetch_instruction_byte(5);
        uint32_t absolute = *(uint32_t*)abs;

        instruction->name = "mov.b";
        instruction->op1.set_register_direct8(b1 & 0x0f);
        instruction->op2.set_absolute_address24(absolute);

        instruction->parser = h8instructions::mov::absolute_address_24_b_parse;
        instruction->runner = h8instructions::mov::absolute_address_24_b_run;
    }
    default: return;
    }
}

int absolute_address_24_b_run(H8300H* h8, Instruction* instruction)
{
    addressing_mode_t mode = instruction->op1.get_mode();

    switch (mode) {
    case addressing_mode_t::AbsoluteAddress24:
        return -1;
    case addressing_mode_t::RegisterDirect8: {
        const Register8& src = h8->reg8[instruction->op1.get_register_direct8()];
        uint32_t absolute = instruction->op2.get_absolute_address24();

        uint8_t value = src.get();
        h8->mcu.write8(absolute, value);

        h8instructions::mov::update_ccr<int8_t>(h8, (int8_t)value);
        h8->pc += 6;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

// void absolute_address_24_w_parse(H8300H* h8, Instruction* instruction);
// int absolute_address_24_w_run(H8300H* h8, Instruction* instruction);

// void absolute_address_24_l_parse(H8300H* h8, Instruction* instruction);
// int absolute_address_24_l_run(H8300H* h8, Instruction* instruction);


}
}
