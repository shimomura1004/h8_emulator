#ifndef _INSTRUCTIONS_AND_INCLUDED_
#define _INSTRUCTIONS_AND_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

// TODO: andl -> and_
namespace h8instructions {
namespace andl {

void immediate_b_parse(H8Board* h8, Instruction& instruction);
int immediate_b_run(H8Board *h8, Instruction& instruction);

void immediate_w_parse(H8Board *h8, Instruction& instruction);
int immediate_w_run(H8Board *h8, Instruction& instruction);

void immediate_l_parse(H8Board *h8, Instruction& instruction);
int immediate_l_run(H8Board *h8, Instruction& instruction);

}
}

#endif
