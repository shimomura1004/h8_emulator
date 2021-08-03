#ifndef _INSTRUCTIONS_EXTS_INCLUDED_
#define _INSTRUCTIONS_EXTS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace exts {

void exts_w_parse(H8Board* h8, Instruction& instruction);
int exts_w_run(H8Board* h8, Instruction& instruction);
void exts_l_parse(H8Board* h8, Instruction& instruction);
int exts_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
