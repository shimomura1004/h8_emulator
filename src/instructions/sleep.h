#ifndef _INSTRUCTIONS_SLEEP_INCLUDED_
#define _INSTRUCTIONS_SLEEP_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace sleep {

void sleep_parse(H8Board* h8, Instruction& instruction);
int sleep_run(H8Board* h8, Instruction& instruction);

}
}

#endif

