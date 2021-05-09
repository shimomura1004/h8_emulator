#ifndef _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_

#include "h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

// mov.b Rs,Rd
void register_direct_b_parse(H8Board* h8, Instruction& instruction);
int register_direct_b_run(H8Board* h8, Instruction& instruction);

// mov.w Rs,Rd
void register_direct_w_parse(H8Board* h8, Instruction& instruction);
int register_direct_w_run(H8Board* h8, Instruction& instruction);

// mov.l ERs,ERd
void register_direct_l_parse(H8Board* h8, Instruction& instruction);
int register_direct_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
