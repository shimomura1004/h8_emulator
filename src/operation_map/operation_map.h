#ifndef _OPERATION_MAP_INCLUDED_
#define _OPERATION_MAP_INCLUDED_

#include "board/h8board.h"
#include "instructions/instruction.h"

namespace operation_map {
instruction_parser_t lookup(H8Board* h8300h);
}

#endif
