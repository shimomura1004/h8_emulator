#ifndef _INSTRUCTIONS_SHLL_INCLUDED_
#define _INSTRUCTIONS_SHLL_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace shll {

void shll_b_parse(H8Board* h8, Instruction& instruction);
int shll_b_run(H8Board* h8, Instruction& instruction);
void shll_w_parse(H8Board* h8, Instruction& instruction);
int shll_w_run(H8Board* h8, Instruction& instruction);
void shll_l_parse(H8Board* h8, Instruction& instruction);
int shll_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
