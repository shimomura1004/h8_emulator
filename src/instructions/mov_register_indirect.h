#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

void register_indirect_b_parse(H8Board* h8, Instruction& instruction);
int register_indirect_b_run(H8Board* h8, Instruction& instruction);
void register_indirect_w_parse(H8Board* h8, Instruction& instruction);
int register_indirect_w_run(H8Board* h8, Instruction& instruction);
void register_indirect_l_parse(H8Board* h8, Instruction& instruction);
int register_indirect_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
