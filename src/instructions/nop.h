#ifndef _INSTRUCTIONS_NOP_INCLUDED_
#define _INSTRUCTIONS_NOP_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace nop {

void nop_parse(H8Board* h8, Instruction& instruction);
int nop_run(H8Board* h8, Instruction& instruction);

}
}

#endif
