#ifndef _INSTRUCTIONS_XOR_INCLUDED_
#define _INSTRUCTIONS_XOR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace xorl {

// TODO: 一般的な ccr の更新、他と同じ
template<class T>
void update_ccr(H8Board* h8, T value)
{
    if (value < 0) {
        h8->cpu.ccr().set_n();
    } else {
        h8->cpu.ccr().clear_n();
    }

    if (value == 0) {
        h8->cpu.ccr().set_z();
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();
}

void xor_immediate_b_parse(H8Board* h8, Instruction& instruction);
int xor_immediate_b_run(H8Board* h8, Instruction& instruction);
void xor_register_direct_b_parse(H8Board* h8, Instruction& instruction);
int xor_register_direct_b_run(H8Board* h8, Instruction& instruction);
void xor_register_direct_l_parse(H8Board* h8, Instruction& instruction);
int xor_register_direct_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
