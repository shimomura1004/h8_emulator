#ifndef _INSTRUCTIONS_JMP_INCLUDED_
#define _INSTRUCTIONS_JMP_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace jmp {

void jmp_register_indirect_parse(H8Board* h8, Instruction& instruction);
int jmp_register_indirect_run(H8Board* h8, Instruction& instruction);

}
}

#endif


