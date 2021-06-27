#include "divxs.h"

namespace h8instructions {
namespace divxs {

void divxs_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);

    instruction.name = "divxs.w";
    instruction.op1.set_register_direct16((b3 & 0xf0) >> 4);
    instruction.op2.set_register_direct32(b3 & 0x07);

    instruction.parser = divxs_w_parse;
    instruction.runner = divxs_w_run;
}

int divxs_w_run(H8Board* h8, Instruction& instruction)
{
    const Register16& src = h8->cpu.reg16(instruction.op1.get_register_direct16());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    // 被除数
    int32_t dst_value = dst.get();
    bool dst_is_positive = dst_value >= 0;
    // 除数
    int16_t src_value = src.get();
    bool src_is_positive = src_value >= 0;

    bool quotient_is_positive = (dst_is_positive && src_is_positive) ||
                                (!dst_is_positive && !src_is_positive);

    if (src_value == 0) {
        // 0除算の場合、結果は不定
        h8->cpu.ccr().set_z();
        h8->cpu.pc() += 4;
        return 0;
    }

    int16_t quotient = dst_value / src_value;
    int16_t remainder = dst_value % src_value;

    // 上位16ビットに剰余、下位16ビットに商を入れる
    // H8 の仕様では剰余の符号は被除数と一致するとある
    // C/C++ の剰余はこの仕様に一致するため特に処置は必要ない
    dst.set((remainder << 16) | (int32_t)quotient);

    quotient_is_positive ? h8->cpu.ccr().clear_n() : h8->cpu.ccr().set_n();
    h8->cpu.ccr().clear_z();

    h8->cpu.pc() += 4;

    return 0;
}

}
}
