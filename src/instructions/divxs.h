#ifndef _INSTRUCTIONS_DIVXS_INCLUDED_
#define _INSTRUCTIONS_DIVXS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace divxs {

void divxs_w_parse(H8Board* h8, Instruction& instruction);
int divxs_w_run(H8Board* h8, Instruction& instruction);

}
}

#endif
