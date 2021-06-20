#ifndef _INSTRUCTIONS_JSR_INCLUDED_
#define _INSTRUCTIONS_JSR_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace jsr {

void jsr_register_indirect_parse(H8Board* h8, Instruction& instruction);
int jsr_register_indirect_run(H8Board* h8, Instruction& instruction);
void jsr_absolute_address_parse(H8Board* h8, Instruction& instruction);
int jsr_absolute_address_run(H8Board* h8, Instruction& instruction);

}
}

#endif