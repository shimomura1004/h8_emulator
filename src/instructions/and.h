#ifndef _INSTRUCTIONS_AND_INCLUDED_
#define _INSTRUCTIONS_AND_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace andl {

int and_immediate_w(H8Board *h8);
int and_immediate_l(H8Board *h8);


void and_immediate_b_parse(H8Board* h8, Instruction& instruction);
int and_immediate_b_run(H8Board *h8, Instruction& instruction);

void and_immediate_w_parse(H8Board *h8, Instruction& instruction);
int and_immediate_w_run(H8Board *h8, Instruction& instruction);

void and_immediate_l_parse(H8Board *h8, Instruction& instruction);
int and_immediate_l_run(H8Board *h8, Instruction& instruction);

}
}

#endif
