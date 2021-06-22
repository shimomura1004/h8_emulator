#ifndef _INSTRUCTIONS_MULXS_INCLUDED_
#define _INSTRUCTIONS_MULXS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mulxs {

void mulxs_b_parse(H8Board* h8, Instruction& instruction);
int mulxs_b_run(H8Board* h8, Instruction& instruction);
void mulxs_w_parse(H8Board* h8, Instruction& instruction);
int mulxs_w_run(H8Board* h8, Instruction& instruction);

}
}

#endif
