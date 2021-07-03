#ifndef _INSTRUCTIONS_BTST_INCLUDED_
#define _INSTRUCTIONS_BTST_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace btst {

void btst_register_direct_imm_parse(H8Board* h8, Instruction& instruction);
int btst_register_direct_imm_run(H8Board* h8, Instruction& instruction);

}
}

#endif

