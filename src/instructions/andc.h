#ifndef _INSTRUCTIONS_ANDC_INCLUDED_
#define _INSTRUCTIONS_ANDC_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace andc {

void andc_parse(H8Board* h8, Instruction& instruction);
int andc_run(H8Board* h8, Instruction& instruction);

}
}

#endif
