#ifndef _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_

#include "h8300h.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

int register_direct_b(H8300H* h8);
int register_direct_w(H8300H* h8);
int register_direct_l(H8300H* h8);

// mov.b Rs,Rd
void register_direct_b_parse(H8300H* h8, Instruction* instruction);
int register_direct_b_run(H8300H* h8, Instruction* instruction);

// mov.w Rs,Rd
void register_direct_w_parse(H8300H* h8, Instruction* instruction);
int register_direct_w_run(H8300H* h8, Instruction* instruction);

// mov.l ERs,ERd
void register_direct_l_parse(H8300H* h8, Instruction* instruction);
int register_direct_l_run(H8300H* h8, Instruction* instruction);

}
}

#endif
