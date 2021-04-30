#include "mov_register_indirect.h"
#include "mov.h"

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

namespace h8instructions {
namespace mov {

void register_indirect_b_parse(H8300H* h8, Instruction* instruction)
{
    const uint8_t b1 = h8->fetch_instruction_byte(1);
    const uint8_t b1_msb = b1 & 0x80;

    instruction->name = "mov.b";
    instruction->parser = h8instructions::mov::register_indirect_b_parse;
    instruction->runner = h8instructions::mov::register_indirect_b_run;

    if (b1_msb) {
        // Rs,@ERd
        instruction->op1.set_register_direct8(b1 & 0x0f);
        instruction->op2.set_register_indirect((b1 & 0x70) >> 4);
    } else {
        // @ERs,Rd
        instruction->op1.set_register_indirect((b1 & 0x70) >> 4);
        instruction->op2.set_register_direct8(b1 & 0x0f);
    }
}

int register_indirect_b_run(H8300H* h8, Instruction* instruction)
{
    addressing_mode_t mode = instruction->op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterDirect8: {
        // Rs,@ERd
        const Register8& src = h8->reg8[instruction->op1.get_register_direct8()];
        Register32& dst = h8->reg[instruction->op2.get_register_indirect()];

        uint8_t value = src.get();
        uint32_t address = dst.get();
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);

        h8->pc += 2;

        return 0;
    }
    case addressing_mode_t::RegisterIndirect: {
        // @ERs,Rd
        const Register32& src = h8->reg[instruction->op1.get_register_indirect()];
        Register8& dst = h8->reg8[instruction->op2.get_register_direct8()];

        uint32_t address = src.get();
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);

        h8->pc += 2;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}


    return 0;
}

// void register_indirect_w_parse(H8300H* h8, Instruction* instruction);
// int register_indirect_w_run(H8300H* h8, Instruction* instruction);
// void register_indirect_l_parse(H8300H* h8, Instruction* instruction);
// int register_indirect_l_run(H8300H* h8, Instruction* instruction);

}
}
