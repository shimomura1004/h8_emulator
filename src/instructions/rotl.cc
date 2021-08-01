#include "rotl.h"

namespace h8instructions {
namespace rotl {

void rotl_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "rotl.b";
    instruction.op1.set_register_direct8(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = rotl_b_parse;
    instruction.runner = rotl_b_run;
}

int rotl_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& reg = h8->cpu.reg8(instruction.op1.get_register_direct8());

    uint8_t result_value = reg.get();
    bool msb = result_value & 0x80;

    result_value = result_value << 1;
    // 0ビット目に MSB の値を入れる
    result_value |= msb ? 0x01 : 0x00;

    reg.set(result_value);

    ((int8_t)result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();
    h8->cpu.ccr().clear_v();
    // CCR.C にキャリーアウトされた値を入れる
    msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
