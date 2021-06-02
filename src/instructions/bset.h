#ifndef _INSTRUCTIONS_BSET_INCLUDED_
#define _INSTRUCTIONS_BSET_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace bset {

int bset_register_indirect(H8Board* h8);


void register_direct_immediate_parse(H8Board* h8, Instruction& instruction);
int register_direct_immediate_run(H8Board* h8, Instruction& instruction);

void bset_register_indirect_parse(H8Board* h8, Instruction& instruction);
int bset_register_indirect_run(H8Board* h8, Instruction& instruction);

}
}

#endif
