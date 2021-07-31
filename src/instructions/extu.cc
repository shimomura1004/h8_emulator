#include "extu.h"

namespace h8instructions {
namespace extu {

template<class T>
static inline void update_ccr(H8Board* h8, T value)
{
    h8->cpu.ccr().clear_n();
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
}

void extu_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "extu.w";
    instruction.op1.set_register_direct16(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = extu_w_parse;
    instruction.runner = extu_w_run;
}

int extu_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& reg = h8->cpu.reg16(instruction.op1.get_register_direct16());

    reg.set(reg.get() & 0x00ff);

    update_ccr<uint16_t>(h8, reg.get());

    h8->cpu.pc() += 2;

    return 0;
}

void extu_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "extu.l";
    instruction.op1.set_register_direct32(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = extu_l_parse;
    instruction.runner = extu_l_run;
}

int extu_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());

    reg.set(reg.get() & 0x0000ffff);

    update_ccr<uint32_t>(h8, reg.get());

    h8->cpu.pc() += 2;

    return 0;
}

}
}
