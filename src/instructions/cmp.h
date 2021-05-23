#ifndef _INSTRUCTIONS_CMP_INCLUDED_
#define _INSTRUCTIONS_CMP_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace cmp {

void cmp_immediate_b_parse(H8Board *h8, Instruction& instruction);
int cmp_immediate_b_run(H8Board *h8, Instruction& instruction);
void cmp_register_direct_b_parse(H8Board* h8, Instruction& instruction);
int cmp_register_direct_b_run(H8Board* h8, Instruction& instruction);

void cmp_immediate_w_parse(H8Board* h8, Instruction& instruction);
int cmp_immediate_w_run(H8Board* h8, Instruction& instruction);
void cmp_register_direct_w_parse(H8Board* h8, Instruction& instruction);
int cmp_register_direct_w_run(H8Board* h8, Instruction& instruction);

void cmp_immediate_l_parse(H8Board* h8, Instruction& instruction);
int cmp_immediate_l_run(H8Board* h8, Instruction& instruction);
void cmp_register_direct_l_parse(H8Board* h8, Instruction& instruction);
int cmp_register_direct_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
