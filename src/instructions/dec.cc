#include "dec.h"

template<int n, class T>
static void update_ccr(H8Board* h8, T dst_value, T result_value)
{
    bool dst_value_nth_bit = dst_value & (1 << (n - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 1));

    (result_value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    (result_value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    bool v = dst_value_nth_bit && !result_value_nth_bit;
    v ? h8->cpu.ccr().set_v() : h8->cpu.ccr().clear_v();
}

namespace h8instructions {
namespace dec {

void dec_b_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "dec.b";
    instruction.op1.set_register_direct8(b1 & 0x0f);
    instruction.op2.set_immediate8(1);

    instruction.parser = dec_b_parse;
    instruction.runner = dec_b_run;
}

int dec_b_run(H8Board* h8, Instruction& instruction)
{
    Register8& reg = h8->cpu.reg8(instruction.op1.get_register_direct8());

    int8_t dst_value = reg.get();
    int8_t result_value = dst_value - instruction.op2.get_immediate8();
    reg.set(result_value);

    ::update_ccr<8, int8_t>(h8, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

void dec_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "dec.w";
    instruction.op1.set_register_direct16(b1 & 0x0f);
    switch (b1 & 0xf0) {
    case 0x50:
        instruction.op2.set_immediate8(1);
        break;
    case 0xd0:
        instruction.op2.set_immediate8(2);
        break;
    default:
        fprintf(stderr, "Unknown dec.w format\n");
        return;
    }

    instruction.parser = dec_w_parse;
    instruction.runner = dec_w_run;
}

int dec_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& reg = h8->cpu.reg16(instruction.op1.get_register_direct16());

    int16_t dst_value = reg.get();
    int16_t result_value = dst_value - instruction.op2.get_immediate8();
    reg.set(result_value);

    ::update_ccr<16, int16_t>(h8, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

void dec_l_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "dec.l";
    instruction.op1.set_register_direct32(b1 & 0x07);
    switch (b1 & 0xf0) {
    case 0x70:
        instruction.op2.set_immediate8(1);
        break;
    case 0xf0:
        instruction.op2.set_immediate8(2);
        break;
    default:
        fprintf(stderr, "Unknown dec.l format\n");
        return;
    }

    instruction.parser = dec_l_parse;
    instruction.runner = dec_l_run;
}

int dec_l_run(H8Board* h8, Instruction& instruction)
{
    Register32& dst = h8->cpu.reg32(instruction.op1.get_register_direct32());

    int32_t dst_value = dst.get();
    int32_t result_value = dst_value - instruction.op2.get_immediate8();
    dst.set(result_value);

    ::update_ccr<32, int32_t>(h8, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

}
}
