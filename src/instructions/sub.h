#ifndef _INSTRUCTIONS_SUB_INCLUDED_
#define _INSTRUCTIONS_SUB_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace sub {

int sub_b(H8Board *h8);
int sub_w(H8Board *h8);
int sub_l(H8Board *h8);
// int sub_immediate_l(H8Board *h8);

template<class T>
void update_ccr(H8Board* h8, T src_value, T dst_value, T result_value)
{
    constexpr int n = sizeof(T) * 8;
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = ( src_value_nth_bit && !dst_value_nth_bit) ||
             (!dst_value_nth_bit &&  result_value_nth_bit) ||
             ( src_value_nth_bit &&  result_value_nth_bit);
    h ? h8->cpu.ccr().set_h() : h8->cpu.ccr().clear_h();

    result_value < 0 ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n();
    result_value == 0 ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = (!src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             ( src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->cpu.ccr().set_v() : h8->cpu.ccr().clear_v();

    bool c = ( src_value_mth_bit && !dst_value_mth_bit) ||
             (!dst_value_mth_bit &&  result_value_mth_bit) ||
             ( src_value_mth_bit &&  result_value_mth_bit);
    c ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c();
}

void sub_immediate_l_parse(H8Board* h8, Instruction& instruction);
int sub_immediate_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
