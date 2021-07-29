#ifndef _INSTRUCTIONS_SHAR_INCLUDED_
#define _INSTRUCTIONS_SHAR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace shar {

void shar_w_parse(H8Board* h8, Instruction& instruction);
int shar_w_run(H8Board* h8, Instruction& instruction);
void shar_l_parse(H8Board* h8, Instruction& instruction);
int shar_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
