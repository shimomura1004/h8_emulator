#ifndef _INSTRUCTIONS_DEC_INCLUDED_
#define _INSTRUCTIONS_DEC_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace dec {

void dec_b_parse(H8Board* h8, Instruction& instruction);
int dec_b_run(H8Board* h8, Instruction& instruction);
void dec_w_parse(H8Board* h8, Instruction& instruction);
int dec_w_run(H8Board* h8, Instruction& instruction);
void dec_l_parse(H8Board* h8, Instruction& instruction);
int dec_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
