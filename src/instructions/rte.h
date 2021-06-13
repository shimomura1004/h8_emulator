#ifndef _INSTRUCTIONS_RTE_INCLUDED_
#define _INSTRUCTIONS_RTE_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace rte {

void rte_parse(H8Board* h8, Instruction& instruction);
int rte_run(H8Board* h8, Instruction& instruction);

}
}

#endif
