#ifndef _INSTRUCTIONS_EXTU_INCLUDED_
#define _INSTRUCTIONS_EXTU_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace extu {

void extu_w_parse(H8Board* h8, Instruction& instruction);
int extu_w_run(H8Board* h8, Instruction& instruction);
void extu_l_parse(H8Board* h8, Instruction& instruction);
int extu_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
