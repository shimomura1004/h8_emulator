#ifndef _INSTRUCTIONS_ROTXL_INCLUDED_
#define _INSTRUCTIONS_ROTXL_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace rotxl {

void rotxl_l_parse(H8Board* h8, Instruction& instruction);
int rotxl_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
