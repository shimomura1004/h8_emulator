#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_

#include "h8300h.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

int register_indirect_l(H8300H* h8);

void register_indirect_b_parse(H8300H* h8, Instruction* instruction);
int register_indirect_b_run(H8300H* h8, Instruction* instruction);
void register_indirect_w_parse(H8300H* h8, Instruction* instruction);
int register_indirect_w_run(H8300H* h8, Instruction* instruction);
void register_indirect_l_parse(H8300H* h8, Instruction* instruction);
int register_indirect_l_run(H8300H* h8, Instruction* instruction);

}
}

#endif
