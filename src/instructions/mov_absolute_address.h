#ifndef _INSTRUCTIONS_MOV_ABSOLUTE_ADDRESS_INCLUDED_
#define _INSTRUCTIONS_MOV_ABSOLUTE_ADDRESS_INCLUDED_

#include "h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace mov {

void absolute_address_24_b_parse(H8Board* h8, Instruction& instruction);
int absolute_address_24_b_run(H8Board* h8, Instruction& instruction);

void absolute_address_24_w_parse(H8Board* h8, Instruction& instruction);
int absolute_address_24_w_run(H8Board* h8, Instruction& instruction);

void absolute_address_24_l_parse(H8Board* h8, Instruction& instruction);
int absolute_address_24_l_run(H8Board* h8, Instruction& instruction);

}
}

#endif
