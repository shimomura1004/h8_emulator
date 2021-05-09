#ifndef _INSTRUCTIONS_JSR_INCLUDED_
#define _INSTRUCTIONS_JSR_INCLUDED_

#include "board/h8board.h"

namespace h8instructions {
namespace jsr {

int jsr_register_indirect(H8Board* h8);
int jsr_absolute_address(H8Board* h8);

}
}

#endif