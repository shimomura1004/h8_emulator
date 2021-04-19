#ifndef _OPERATION_MAP_6_INCLUDED_
#define _OPERATION_MAP_6_INCLUDED_

#include "operation_map.h"
#include "instructions/instruction.h"

namespace operation_map {
instruction_handler_t lookup_6x(H8300H* h8300h);
instruction_handler_t lookup_67(H8300H* h8300h);
}

namespace operation_map2 {
instruction_parser_t lookup_6x(H8300H* h8300h);
instruction_parser_t lookup_67(H8300H* h8300h);
}

#endif
