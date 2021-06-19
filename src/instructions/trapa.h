#ifndef _INSTRUCTIONS_TRAPA_INCLUDED_
#define _INSTRUCTIONS_TRAPA_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace trapa {

void trapa_parse(H8Board* h8, Instruction& instruction);
int trapa_run(H8Board* h8, Instruction& instruction);

}
}

#endif
