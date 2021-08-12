#ifndef _OPERATION_MAP_INCLUDED_
#define _OPERATION_MAP_INCLUDED_

#include "board/h8board.h"
#include "instructions/instruction.h"

typedef int(*instruction_handler_t)(H8Board*);

namespace operation_map2 {
instruction_parser_t lookup(H8Board* h8300h);
}

#endif
