#ifndef _INSTRUCTIONS_ORC_INCLUDED_
#define _INSTRUCTIONS_ORC_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace orc {

void orc_parse(H8Board* h8, Instruction& instruction);
int orc_run(H8Board* h8, Instruction& instruction);

}
}

#endif
