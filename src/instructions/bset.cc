#include "bset.h"
#include "bit_helper.h"

int h8instructions::bset::bset_register_indirect(H8Board* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t reg_index = (b1 & 0x70) >> 4;
    const Register32& reg = h8->cpu.reg32(reg_index);

    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t imm = (b3 & 0x70) >> 4;

    uint32_t address = reg.get();
    uint8_t value = h8->mcu.read8(address);

    uint8_t mask = (1 << imm);
    value |= mask;
    h8->mcu.write8(address, value);

    h8->cpu.pc() += 4;

    return 0;
}


namespace h8instructions {
namespace bset {

void register_direct_immediate_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::bit::parse_register_direct_immediate<register_direct_immediate_parse, register_direct_immediate_run>(h8, instruction, "bset");
}

int register_direct_immediate_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::bit::run_register_direct_immediate(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 | op2;});
}

void bset_register_indirect_parse(H8Board* h8, Instruction& instruction);
int bset_register_indirect_run(H8Board* h8, Instruction& instruction);

}
}
