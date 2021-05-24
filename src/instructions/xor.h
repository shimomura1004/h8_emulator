#ifndef _INSTRUCTIONS_XOR_INCLUDED_
#define _INSTRUCTIONS_XOR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace xorl {

int xor_register_direct_b(H8Board* h8);
int xor_register_direct_l(H8Board* h8);


void xor_immediate_b_parse(H8Board* h8, Instruction& instruction);
int xor_immediate_b_run(H8Board* h8, Instruction& instruction);
void xor_register_direct_b_parse(H8Board* h8, Instruction& instruction);
int xor_register_direct_b_run(H8Board* h8, Instruction& instruction);
void xor_register_direct_l_parse(H8Board* h8, Instruction& instruction);
int xor_register_direct_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
