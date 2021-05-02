#include "jmp.h"

int h8instructions::jmp::jmp_register_indirect(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t register_index = (b1 & 0x70) >> 4;
    const Register32& reg = h8->cpu.reg32(register_index);

    uint32_t address = reg.get();

    h8->cpu.pc() = address;

    return 0;
}
