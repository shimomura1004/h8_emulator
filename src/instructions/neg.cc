#include "neg.h"

template<int n, class T>
static void update_ccr(H8Board* h8, T dst_value, T result_value)
{
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = (dst_value_nth_bit || result_value_nth_bit);
    h ? h8->cpu.ccr().set_h() : h8->cpu.ccr().clear_h();

    result_value < 0 ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    result_value == 0 ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = dst_value_mth_bit &&  result_value_mth_bit;
    v ? h8->cpu.ccr().set_v() : h8->cpu.ccr().clear_v();

    bool c = dst_value_mth_bit || result_value_mth_bit;
    c ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_v();
}

namespace h8instructions {
namespace neg {

void neg_w_parse(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "neg.w";
    instruction.op1.set_register_direct16(b1 & 0x0f);
    instruction.op2.set_not_used();

    instruction.parser = neg_w_parse;
    instruction.runner = neg_w_run;
}

int neg_w_run(H8Board* h8, Instruction& instruction)
{
    Register16& dst = h8->cpu.reg16(instruction.op1.get_register_direct16());

    int16_t dst_value = dst.get();
    int16_t result_value = -dst_value;

    if (dst_value == (int16_t)0x8000) {
        // 「実行前のRdの内容がH'8000の場合 の結果はH'8000となります」
        result_value = dst_value;
    }
    dst.set(result_value);

    ::update_ccr<16, int16_t>(h8, dst_value, result_value);

    h8->cpu.pc() += 2;

    return 0;
}

}
}
