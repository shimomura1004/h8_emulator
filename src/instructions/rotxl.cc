#include "rotxl.h"

namespace h8instructions {
namespace rotxl {

void rotxl_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "rotxl.l";
    instruction.op1.set_register_direct32(b1 & 0x07);
    instruction.op2.set_not_used();

    instruction.parser = rotxl_l_parse;
    instruction.runner = rotxl_l_run;
}

int rotxl_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& reg = h8->cpu.reg32(instruction.op1.get_register_direct32());

    uint32_t result_value = reg.get();
    bool msb = result_value & 0x80000000;

    result_value = result_value << 1;
    // 0ビット目に CCR.C の値を入れる
    result_value |= h8->cpu.ccr().c() ? 0x00000001 : 0x00000000;

    reg.set(result_value);

    ((int32_t)result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    // CCR.C にキャリーアウトされた値を入れる
    msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
