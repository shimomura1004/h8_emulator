#ifndef _INSTRUCTIONS_SUB_INCLUDED_
#define _INSTRUCTIONS_SUB_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace sub {

int sub_b(H8300H *h8);
int sub_w(H8300H *h8);
int sub_l(H8300H *h8);
int sub_immediate_l(H8300H *h8);

template<int n, class T>
void update_ccr(H8300H* h8, T src_value, T dst_value, T result_value)
{
    bool src_value_nth_bit = src_value & (1 << (n - 4 - 1));
    bool dst_value_nth_bit = dst_value & (1 << (n - 4 - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 4 - 1));

    bool h = ( src_value_nth_bit && !dst_value_nth_bit) ||
             (!dst_value_nth_bit &&  result_value_nth_bit) ||
             ( src_value_nth_bit &&  result_value_nth_bit);
    h ? h8->ccr.set_h() : h8->ccr.clear_h();

    result_value < 0 ? h8->ccr.set_n() : h8->ccr.clear_n();
    result_value == 0 ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool src_value_mth_bit = src_value & (1 << (n - 1));
    bool dst_value_mth_bit = dst_value & (1 << (n - 1));
    bool result_value_mth_bit = result_value & (1 << (n - 1));

    bool v = (!src_value_mth_bit &&  dst_value_mth_bit && !result_value_mth_bit) ||
             ( src_value_mth_bit && !dst_value_mth_bit &&  result_value_mth_bit);
    v ? h8->ccr.set_v() : h8->ccr.clear_v();

    bool c = ( src_value_mth_bit && !dst_value_mth_bit) ||
             (!dst_value_mth_bit &&  result_value_mth_bit) ||
             ( src_value_mth_bit &&  result_value_mth_bit);
    c ? h8->ccr.set_c() : h8->ccr.clear_v();
}

}
}

#endif
