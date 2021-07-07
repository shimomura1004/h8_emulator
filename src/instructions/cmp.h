#ifndef _INSTRUCTIONS_CMP_INCLUDED_
#define _INSTRUCTIONS_CMP_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace cmp {

void immediate_b_parse(H8Board *h8, Instruction& instruction);
int immediate_b_run(H8Board *h8, Instruction& instruction);
void register_direct_b_parse(H8Board* h8, Instruction& instruction);
int register_direct_b_run(H8Board* h8, Instruction& instruction);

void immediate_w_parse(H8Board* h8, Instruction& instruction);
int immediate_w_run(H8Board* h8, Instruction& instruction);
void register_direct_w_parse(H8Board* h8, Instruction& instruction);
int register_direct_w_run(H8Board* h8, Instruction& instruction);

void immediate_l_parse(H8Board* h8, Instruction& instruction);
int immediate_l_run(H8Board* h8, Instruction& instruction);
void register_direct_l_parse(H8Board* h8, Instruction& instruction);
int register_direct_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
