#ifndef _INSTRUCTIONS_SUBS_INCLUDED_
#define _INSTRUCTIONS_SUBS_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace subs {

void subs_parse(H8Board* h8, Instruction& instruction);
int subs_run(H8Board* h8, Instruction& instruction);

}
}

#endif
