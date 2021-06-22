#include "mulxs.h"



namespace h8instructions {
namespace mulxs {

void mulxs_b_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "mulxs.b";
    instruction.parser = nullptr;
    instruction.runner = nullptr;
}

int mulxs_b_run(H8Board* h8, Instruction& instruction)
{
    return -1;
}

void mulxs_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);

    instruction.name = "mulxs.w";
    instruction.op1.set_register_direct16((b3 & 0xf0) >> 4);
    instruction.op2.set_register_direct32(b3 & 0x07);

    instruction.parser = mulxs_w_parse;
    instruction.runner = mulxs_w_run;
}

int mulxs_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    int16_t src_value = src.get();
    // ER の下位16ビット(= R)を抜き出す
    int16_t dst_value = dst.get() & 0x0000ffff;
    int32_t result_value = (int32_t)dst_value * src_value;

    dst.set(result_value);
    (result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    h8->cpu.pc() += 4;

    return 0;
}

}
}
