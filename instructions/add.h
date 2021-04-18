#ifndef _INSTRUCTIONS_ADD_INCLUDED_
#define _INSTRUCTIONS_ADD_INCLUDED_

#include "h8300h.h"
#include "instruction.h"

namespace h8instructions {
namespace add {

// todo: 消す
int add_immediate_b(H8300H* h8);
int add_register_direct_b(H8300H* h8);

int add_immediate_w(H8300H* h8);
int add_register_direct_w(H8300H* h8);

int add_immediate_l(H8300H* h8);
int add_register_direct_l(H8300H* h8);


// add.b #xx:8,Rd
void add_immediate_b_parse(H8300H* h8, Instruction* instruction);
int add_immediate_b_run(H8300H* h8, Instruction* instruction);

// add.b Rs,Rd
void add_register_direct_b_parse(H8300H* h8, Instruction* instruction);
int add_register_direct_b_run(H8300H* h8, Instruction* instruction);

// add.w #xx:16,Rd
void add_immediate_w_parse(H8300H* h8, Instruction* instruction);
int add_immediate_w_run(H8300H* h8, Instruction* instruction);

// add.w Rs,Rd
void add_register_direct_w_parse(H8300H* h8, Instruction* instruction);
int add_register_direct_w_run(H8300H* h8, Instruction* instruction);

// add.l #xx:32,Rd
void add_immediate_l_parse(H8300H* h8, Instruction* instruction);
int add_immediate_l_run(H8300H* h8, Instruction* instruction);

// add.l ERs,ERd
void add_register_direct_l_parse(H8300H* h8, Instruction* instruction);
int add_register_direct_l_run(H8300H* h8, Instruction* instruction);

}
}

#endif
