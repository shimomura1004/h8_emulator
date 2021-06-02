#ifndef _INSTRUCTIONS_BCLR_INCLUDED_
#define _INSTRUCTIONS_BCLR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace bclr {

void register_direct_immediate_parse(H8Board* h8, Instruction& instruction);
int register_direct_immediate_run(H8Board* h8, Instruction& instruction);

}
}

#endif
