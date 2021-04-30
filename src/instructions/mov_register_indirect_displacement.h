#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_INCLUDED_

#include "h8300h.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement16_l(H8300H* h8);

int register_indirect_with_displacement24_b(H8300H* h8);
int register_indirect_with_displacement24_w(H8300H* h8);
int register_indirect_with_displacement24_l(H8300H* h8);

int register_indirect_with_increment_decrement_b(H8300H* h8);
int register_indirect_with_increment_decrement_l(H8300H* h8);


void register_indirect_with_displacement16_b_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement16_b_run(H8300H* h8, Instruction* instruction);
void register_indirect_with_displacement16_w_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement16_w_run(H8300H* h8, Instruction* instruction);
void register_indirect_with_displacement16_l_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement16_l_run(H8300H* h8, Instruction* instruction);

void register_indirect_with_displacement24_b_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement24_b_run(H8300H* h8, Instruction* instruction);
void register_indirect_with_displacement24_w_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement24_w_run(H8300H* h8, Instruction* instruction);
void register_indirect_with_displacement24_l_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_displacement24_l_run(H8300H* h8, Instruction* instruction);

void register_indirect_with_increment_decrement_b_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_increment_decrement_b_run(H8300H* h8, Instruction* instruction);
void register_indirect_with_increment_decrement_l_parser(H8300H* h8, Instruction* instruction);
int register_indirect_with_increment_decrement_l_run(H8300H* h8, Instruction* instruction);

}
}

#endif
