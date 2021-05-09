#include "mov_register_indirect_displacement16.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

void register_indirect_with_displacement16_b_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    uint8_t disp[2];
    disp[1] = h8->fetch_instruction_byte(2);
    disp[0] = h8->fetch_instruction_byte(3);
    int16_t displacement = *(int16_t*)disp;

    instruction.name = "mov.b";
    instruction.parser = h8instructions::mov::register_indirect_with_displacement16_b_parser;
    instruction.runner = h8instructions::mov::register_indirect_with_displacement16_b_run;

    if (b1_msb) {
        // Rs,@(d:16,ERd)
        instruction.op1.set_register_direct8(b1 & 0x0f);
        instruction.op2.set_register_indirect_with_displacement16((b1 & 0x70) >> 4, displacement);
    } else {
        // @(d:16,ERs),Rd
        instruction.op1.set_register_indirect_with_displacement16((b1 & 0x70) >> 4, displacement);
        instruction.op2.set_register_direct8(b1 & 0x0f);
    }
}

int register_indirect_with_displacement16_b_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterDirect8: {
        // Rs,@(d:16,ERd)
        const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
        Register32& dst = h8->cpu.reg32(instruction.op2.get_register_indirect_with_displacement16_register());
        int16_t displacement = instruction.op2.get_register_indirect_with_displacement16_displacement();

        uint8_t value = src.get();
        uint32_t address = dst.get() + displacement;
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    case addressing_mode_t::RegisterIndirectWithDisplacement16: {
        // @(d:16,ERs),Rd
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_indirect_with_displacement16_register());
        Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());
        int16_t displacement = instruction.op1.get_register_indirect_with_displacement16_displacement();

        uint32_t address = src.get() + displacement;
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

void register_indirect_with_displacement16_w_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1_msb = b1 & 0x80;

    uint8_t disp[2];
    disp[1] = h8->fetch_instruction_byte(2);
    disp[0] = h8->fetch_instruction_byte(3);
    int16_t displacement = *(int16_t*)disp;

    instruction.name = "mov.w";
    instruction.parser = h8instructions::mov::register_indirect_with_displacement16_w_parser;
    instruction.runner = h8instructions::mov::register_indirect_with_displacement16_w_run;

    if (b1_msb) {
        // Rs,@(d:16,ERd)
        instruction.op1.set_register_direct16(b1 & 0x0f);
        instruction.op2.set_register_indirect_with_displacement16((b1 & 0x70) >> 4, displacement);
    } else {
        // @(d:16,ERs),Rd
        instruction.op1.set_register_indirect_with_displacement16((b1 & 0x70) >> 4, displacement);
        instruction.op2.set_register_direct16(b1 & 0x0f);
    }
}

int register_indirect_with_displacement16_w_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterDirect16: {
        // Rs,@(d:16,ERd)
        const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
        Register32& dst = h8->cpu.reg32(instruction.op2.get_register_indirect_with_displacement16_register());
        int16_t displacement = instruction.op2.get_register_indirect_with_displacement16_displacement();

        uint16_t value = src.get();
        uint32_t address = dst.get() + displacement;
        h8->mcu.write16(address, value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    case addressing_mode_t::RegisterIndirectWithDisplacement16: {
        // @(d:16,ERs),Rd
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_indirect_with_displacement16_register());
        Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());
        int16_t displacement = instruction.op1.get_register_indirect_with_displacement16_displacement();

        uint32_t address = src.get() + displacement;
        uint16_t value = h8->mcu.read16(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

void register_indirect_with_displacement16_l_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b3_msb = b3 & 0x80;

    uint8_t disp[2];
    disp[1] = h8->fetch_instruction_byte(4);
    disp[0] = h8->fetch_instruction_byte(5);
    int16_t displacement = *(int16_t*)disp;

    instruction.name = "mov.l";
    instruction.parser = h8instructions::mov::register_indirect_with_displacement16_l_parser;
    instruction.runner = h8instructions::mov::register_indirect_with_displacement16_l_run;

    if (b3_msb) {
        // Rs,@(d:16,ERd)
        uint8_t b3 = h8->fetch_instruction_byte(3);
        instruction.op1.set_register_direct32(b3 & 0x07);
        instruction.op2.set_register_indirect_with_displacement16((b3 & 0x70) >> 4, displacement);
    } else {
        // @(d:16,ERs),Rd
        uint8_t b3 = h8->fetch_instruction_byte(3);
        instruction.op1.set_register_indirect_with_displacement16((b3 & 0x70) >> 4, displacement);
        instruction.op2.set_register_direct32(b3 & 0x07);
    }
}

int register_indirect_with_displacement16_l_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterDirect32: {
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
        const Register32& dst = h8->cpu.reg32(instruction.op2.get_register_indirect_with_displacement16_register());
        int16_t displacement = instruction.op2.get_register_indirect_with_displacement16_displacement();

        int32_t value = src.get();
        uint32_t address = dst.get() + displacement;
        h8->mcu.write32(address, value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);

        h8->cpu.pc() += 6;

        return 0;
    }
    case addressing_mode_t::RegisterIndirectWithDisplacement16: {
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_indirect_with_displacement16_register());
        Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());
        int16_t displacement = instruction.op1.get_register_indirect_with_displacement16_displacement();

        uint32_t address = src.get() + displacement;
        int32_t value = h8->mcu.read32(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);

        h8->cpu.pc() += 6;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

}
}