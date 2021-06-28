#ifndef _INSTRUCTIIONS_ADDS_INCLUDED_
#define _INSTRUCTIIONS_ADDS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace adds {

void adds_parse(H8Board* h8, Instruction& instruction);
int adds_run(H8Board* h8, Instruction& instruction);

}
}

#endif
