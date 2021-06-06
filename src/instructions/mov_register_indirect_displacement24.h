#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT24_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT24_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement24_w(H8Board* h8);
int register_indirect_with_displacement24_l(H8Board* h8);

void register_indirect_with_displacement24_b_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement24_b_run(H8Board* h8, Instruction& instruction);

void register_indirect_with_displacement24_w_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement24_w_run(H8Board* h8, Instruction& instruction);
void register_indirect_with_displacement24_l_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement24_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
