#ifndef _INSTRUCTIONS_BTST_INCLUDED_
#define _INSTRUCTIONS_BTST_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace btst {

void register_direct_imm_parse(H8Board* h8, Instruction& instruction);
int register_direct_imm_run(H8Board* h8, Instruction& instruction);

}
}

#endif

