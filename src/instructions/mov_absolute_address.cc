#include "mov_absolute_address.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

void absolute_address_24_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;

    uint8_t abs[4];
    abs[3] = 0;
    abs[2] = h8->fetch_instruction_byte(3);
    abs[1] = h8->fetch_instruction_byte(4);
    abs[0] = h8->fetch_instruction_byte(5);
    uint32_t absolute = *(uint32_t*)abs;

    instruction.name = "mov.b";
    instruction.parser = h8instructions::mov::absolute_address_24_b_parse;
    instruction.runner = h8instructions::mov::absolute_address_24_b_run;

    switch (b1h) {
    case 0x00:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    case 0x02:
        instruction.op1.set_absolute_address24(absolute);
        instruction.op2.set_register_direct8(b1 & 0x0f);
        return;
    case 0x08:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    case 0x0a:
        instruction.op1.set_register_direct8(b1 & 0x0f);
        instruction.op2.set_absolute_address24(absolute);
        return;
    default:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    }
}

int absolute_address_24_b_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::AbsoluteAddress24: {
        uint32_t absolute = instruction.op1.get_absolute_address24();
        Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

        uint8_t value = h8->mcu.read8(absolute);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, (int8_t)value);
        h8->cpu.pc() += 6;

        return 0;
    }
    case addressing_mode_t::RegisterDirect8: {
        const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
        uint32_t absolute = instruction.op2.get_absolute_address24();

        uint8_t value = src.get();
        h8->mcu.write8(absolute, value);

        h8instructions::mov::update_ccr<int8_t>(h8, (int8_t)value);
        h8->cpu.pc() += 6;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

void absolute_address_24_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;

    uint8_t abs[4];
    abs[3] = 0;
    abs[2] = h8->fetch_instruction_byte(3);
    abs[1] = h8->fetch_instruction_byte(4);
    abs[0] = h8->fetch_instruction_byte(5);
    int32_t absolute = *(int32_t*)abs;

    instruction.name = "mov.w";
    instruction.parser = h8instructions::mov::absolute_address_24_w_parse;
    instruction.runner = h8instructions::mov::absolute_address_24_w_run;

    switch (b1h) {
    case 0x00:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    case 0x02:
        instruction.op1.set_absolute_address24(absolute);
        instruction.op2.set_register_direct16(b1 & 0x0f);
        return;
    case 0x08:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    case 0x0a:
        instruction.op1.set_register_direct16(b1 & 0x0f);
        instruction.op2.set_absolute_address24(absolute);
        return;
    default:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    }
}

int absolute_address_24_w_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::AbsoluteAddress24: {
        Register16& dst = h8->cpu.reg16(instruction.op2.get_register_direct16());
        uint32_t absolute = instruction.op1.get_absolute_address24();

        uint16_t value = h8->mcu.read16(absolute);
        dst.set(value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
        h8->cpu.pc() += 6;

        return 0;
    }
    case addressing_mode_t::RegisterDirect16: {
        const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
        uint32_t absolute = instruction.op2.get_absolute_address24();

        uint16_t value = src.get();
        h8->mcu.write16(absolute, value);

        h8instructions::mov::update_ccr<int16_t>(h8, value);
        h8->cpu.pc() += 6;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

void absolute_address_24_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    uint8_t b3h = (b3 & 0xf0) >> 4;

    uint8_t abs[4];
    abs[3] = 0;
    abs[2] = h8->fetch_instruction_byte(5);
    abs[1] = h8->fetch_instruction_byte(6);
    abs[0] = h8->fetch_instruction_byte(7);
    int32_t absolute = *(int32_t*)abs;

    instruction.name = "mov.l";
    instruction.parser = h8instructions::mov::absolute_address_24_l_parse;
    instruction.runner = h8instructions::mov::absolute_address_24_l_run;

    switch (b3h) {
    case 0x02:
        instruction.op1.set_absolute_address24(absolute);
        instruction.op2.set_register_direct32(b3 & 0x07);
        return;
    case 0x0a:
        instruction.op1.set_register_direct32(b3 & 0x07);
        instruction.op2.set_absolute_address24(absolute);
        return;
    default:
        instruction.parser = nullptr;
        instruction.runner = nullptr;
        return;
    }
}

int absolute_address_24_l_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::AbsoluteAddress24: {
        Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());
        uint32_t absolute = instruction.op1.get_absolute_address24();

        int32_t value = h8->mcu.read32(absolute);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
        h8->cpu.pc() += 8;

        return 0;
    }
    case addressing_mode_t::RegisterDirect32: {
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
        uint32_t absolute = instruction.op2.get_absolute_address24();

        uint32_t value = src.get();
        h8->mcu.write32(absolute, value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
        h8->cpu.pc() += 8;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

}
}
