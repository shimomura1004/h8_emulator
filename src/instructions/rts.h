#ifndef _INSTRUCTIONS_RTS_INCLUDED_
#define _INSTRUCTIONS_RTS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace rts {

void rts_parse(H8Board* h8, Instruction& instruction);
int rts_run(H8Board* h8, Instruction& instruction);

}
}

#endif
