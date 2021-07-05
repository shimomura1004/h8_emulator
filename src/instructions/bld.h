#ifndef _INSTRUCTIONS_BLD_INCLUDED_
#define _INSTRUCTIONS_BLD_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace bld {

void bld_register_direct_parse(H8Board* h8, Instruction& instruction);
int bld_register_direct_run(H8Board* h8, Instruction& instruction);

}
}

#endif
