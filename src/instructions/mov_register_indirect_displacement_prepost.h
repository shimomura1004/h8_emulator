#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_PREPOST_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_PREPOST_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

void register_indirect_with_increment_decrement_b_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_increment_decrement_b_run(H8Board* h8, Instruction& instruction);
void register_indirect_with_increment_decrement_l_parser(H8Board* h8, Instruction& instruction);
int register_indirect_with_increment_decrement_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
