#ifndef _OPERATION_MAP_7_INCLUDED_
#define _OPERATION_MAP_7_INCLUDED_

#include "operation_map.h"
#include "instructions/instruction.h"

namespace operation_map {
instruction_handler_t lookup_7x(H8Board* h8300h);
}

namespace operation_map2 {
instruction_parser_t lookup_7x(H8Board* h8300h);
instruction_parser_t lookup_74(H8Board* h8300h);
instruction_parser_t lookup_75(H8Board* h8300h);
instruction_parser_t lookup_76(H8Board* h8300h);
instruction_parser_t lookup_77(H8Board* h8300h);
instruction_parser_t lookup_79(H8Board* h8300h);
instruction_parser_t lookup_7A(H8Board* h8300h);
}

#endif
