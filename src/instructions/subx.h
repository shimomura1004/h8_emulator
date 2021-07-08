#ifndef _INSTRUCTIONS_SUBX_INCLUDED_
#define _INSTRUCTIONS_SUBX_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace subx {

void register_direct_parse(H8Board* h8, Instruction& instruction);
int register_direct_run(H8Board* h8, Instruction& instruction);

}
}

#endif
