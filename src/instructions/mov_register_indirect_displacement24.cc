#include "mov_register_indirect_displacement24.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement24_l(H8Board* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b5 = h8->fetch_instruction_byte(5);
    int32_t displacement = h8instructions::parse_immediate<int32_t>(h8, 7, 3, false);

    if ((b3 & 0x80) == 0) {
        // @(d:24,ERs),ERd
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b5 & 0x07);
        Register32& src = h8->cpu.reg32(src_register_index);
        Register32& dst = h8->cpu.reg32(dst_register_index);

        uint32_t address = src.get() + displacement;
        uint32_t value = h8->mcu.read32(address);

        dst.set(value);

        h8instructions::update_ccr_nzv<int32_t>(h8, value);
    } else {
        // ERs,@(d:24,ERd)
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        uint8_t src_register_index = (b5 & 0x07);
        const Register32& dst = h8->cpu.reg32(dst_register_index);
        const Register32& src = h8->cpu.reg32(src_register_index);

        uint32_t value = src.get();
        uint32_t address = dst.get() + displacement;
        h8->mcu.write32(address, value);

        h8instructions::update_ccr_nzv<int32_t>(h8, value);
    }

    h8->cpu.pc() += 10;

    return 0;
}

int register_indirect_with_displacement24_w(H8Board* h8)
{
    return -1;
}

}
}

namespace h8instructions {
namespace mov {

void register_indirect_with_displacement24_b_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b3h = (b3 & 0xf0) >> 4;
    int32_t displacement = h8instructions::parse_immediate<int32_t>(h8, 5, 3, false);

    instruction.name = "mov.b";
    instruction.parser = register_indirect_with_displacement24_b_parser;
    instruction.runner = register_indirect_with_displacement24_b_run;

    if (b3h == 0x02) {
        // @(d:24,ERs),Rd
        instruction.op1.set_register_indirect_with_displacement24((b1 & 0x70) >> 4, displacement);
        instruction.op2.set_register_direct8(b3 & 0x0f);
    } else if (b3h == 0x0a) {
        // Rs,@(d:24,ERd)
        instruction.op1.set_register_direct8(b3 & 0x0f);
        instruction.op2.set_register_indirect_with_displacement24((b1 & 0x70) >> 4, displacement);
    } else {
        fprintf(stderr, "Unknown mov.b format\n");
        instruction.parser = nullptr;
        instruction.runner = nullptr;
    }
}

int register_indirect_with_displacement24_b_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterIndirectWithDisplacement24: {
        // @(d:24,ERs),Rd
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_indirect_with_displacement24_register());
        Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());
        int32_t displacement = instruction.op1.get_register_indirect_with_displacement24_displacement();

        uint32_t address = src.get() + displacement;
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::update_ccr_nzv<int8_t>(h8, value);

        h8->cpu.pc() += 8;

        return 0;
    }
    case addressing_mode_t::RegisterDirect8: {
        // Rs,@(d:24,ERd)
        const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
        const Register32& dst = h8->cpu.reg32(instruction.op2.get_register_indirect_with_displacement24_register());
        int32_t displacement = instruction.op2.get_register_indirect_with_displacement24_displacement();

        uint8_t value = src.get();
        uint32_t address = dst.get() + displacement;
        h8->mcu.write8(address, value);

        h8instructions::update_ccr_nzv<int8_t>(h8, value);

        h8->cpu.pc() += 8;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

// void register_indirect_with_displacement24_w_parser(H8Board* h8, Instruction& instruction);
// int register_indirect_with_displacement24_w_run(H8Board* h8, Instruction& instruction);
// void register_indirect_with_displacement24_l_parser(H8Board* h8, Instruction& instruction);
// int register_indirect_with_displacement24_l_run(H8Board* h8, Instruction& instruction);

}
}