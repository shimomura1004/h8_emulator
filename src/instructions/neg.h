#ifndef _INSTRUCTIONS_NEG_INCLUDED_
#define _INSTRUCTIONS_NEG_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace neg {

void neg_w_parse(H8Board* h8, Instruction& instruction);
int neg_w_run(H8Board* h8, Instruction& instruction);

}
}

#endif
