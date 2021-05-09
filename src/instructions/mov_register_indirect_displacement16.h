#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT16_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT16_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

void register_indirect_with_displacement16_b_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement16_b_run(H8Board* h8, Instruction& instruction);
void register_indirect_with_displacement16_w_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement16_w_run(H8Board* h8, Instruction& instruction);
void register_indirect_with_displacement16_l_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_displacement16_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
