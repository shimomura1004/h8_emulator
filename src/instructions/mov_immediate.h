#ifndef _INSTRUCTIONS_MOV_IMMEDIATE_INCLUDED_
#define _INSTRUCTIONS_MOV_IMMEDIATE_INCLUDED_

#include "h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

void immediate_b_parse(H8Board* h8, Instruction& instruction);
int immediate_b_run(H8Board* h8, Instruction& instruction);

void immediate_w_parse(H8Board* h8, Instruction& instruction);
int immediate_w_run(H8Board* h8, Instruction& instruction);

void immediate_l_parse(H8Board* h8, Instruction& instruction);
int immediate_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
