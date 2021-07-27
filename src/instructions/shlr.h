#ifndef _INSTRUCTIONS_SHLR_INCLUDED_
#define _INSTRUCTIONS_SHLR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace shlr {

void shlr_b_parse(H8Board* h8, Instruction& instruction);
int shlr_b_run(H8Board* h8, Instruction& instruction);

void shlr_w_parse(H8Board* h8, Instruction& instruction);
int shlr_w_run(H8Board* h8, Instruction& instruction);

void shlr_l_parse(H8Board* h8, Instruction& instruction);
int shlr_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
