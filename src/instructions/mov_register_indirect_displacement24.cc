#include "mov_register_indirect_displacement24.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement24_b(H8Board* h8)
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
        const Register32& src = h8->cpu.reg32(src_register_index);
        Register8& dst = h8->cpu.reg8(dst_register_index);

        uint32_t address = src.get() + disp;
        uint8_t value = h8->mcu.read8(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else if (b3h == 0x0a) {
        // Rs,@(d:24,ERd)
        uint8_t dst_register_index = (b1 & 0x70) >> 4;
        uint8_t src_register_index = (b3 & 0x0f);
        const Register32& dst = h8->cpu.reg32(dst_register_index);
        const Register8& src = h8->cpu.reg8(src_register_index);

        uint8_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write8(address, value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
    } else {
        fprintf(stderr, "Unknown mov.b format\n");
        return -1;
    }

    h8->cpu.pc() += 8;

    return 0;
}


int register_indirect_with_displacement24_l(H8Board* h8)
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
        Register32& src = h8->cpu.reg32(src_register_index);
        Register32& dst = h8->cpu.reg32(dst_register_index);
        uint32_t address = src.get() + disp;
        uint32_t value = h8->mcu.read32(address);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    } else {
        // ERs,@(d:24,ERd)
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        uint8_t src_register_index = (b5 & 0x07);
        const Register32& dst = h8->cpu.reg32(dst_register_index);
        const Register32& src = h8->cpu.reg32(src_register_index);

        uint32_t value = src.get();
        uint32_t address = dst.get() + disp;
        h8->mcu.write32(address, value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
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

// void register_indirect_with_displacement24_b_parser(H8300H* h8, Instruction* instruction);
// int register_indirect_with_displacement24_b_run(H8300H* h8, Instruction* instruction);
// void register_indirect_with_displacement24_w_parser(H8300H* h8, Instruction* instruction);
// int register_indirect_with_displacement24_w_run(H8300H* h8, Instruction* instruction);
// void register_indirect_with_displacement24_l_parser(H8300H* h8, Instruction* instruction);
// int register_indirect_with_displacement24_l_run(H8300H* h8, Instruction* instruction);

}
}