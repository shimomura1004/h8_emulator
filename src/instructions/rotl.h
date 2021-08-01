#ifndef _INSTRUCTIONS_ROTL_INCLUDED_
#define _INSTRUCTIONS_ROTL_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace rotl {

void rotl_b_parse(H8Board* h8, Instruction& instruction);
int rotl_b_run(H8Board* h8, Instruction& instruction);

}
}

#endif
