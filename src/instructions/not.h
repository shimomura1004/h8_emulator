#ifndef _INSTRUCTIONS_NOT_INCLUDED_
#define _INSTRUCTIONS_NOT_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace h8not {

void not_b_parse(H8Board* h8, Instruction& instruction);
int not_b_run(H8Board* h8, Instruction& instruction);
void not_w_parse(H8Board* h8, Instruction& instruction);
int not_w_run(H8Board* h8, Instruction& instruction);
void not_l_parse(H8Board* h8, Instruction& instruction);
int not_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
