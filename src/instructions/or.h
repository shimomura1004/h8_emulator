#ifndef _INSTRUCTIONS_OR_INCLUDED_
#define _INSTRUCTIONS_OR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace orl {

int or_immediate_w(H8Board *h8);

}
}

namespace h8instructions {
namespace orl {

void immediate_b_parse(H8Board* h8, Instruction& instruction);
int immediate_b_run(H8Board* h8, Instruction& instruction);
void immediate_w_parse(H8Board *h8, Instruction& instruction);
int immediate_w_run(H8Board *h8, Instruction& instruction);

}
}

#endif
