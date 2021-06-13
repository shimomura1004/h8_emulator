#ifndef _OPERATION_MAP_1_INCLUDED_
#define _OPERATION_MAP_1_INCLUDED_

#include "operation_map.h"
#include "instructions/instruction.h"

namespace operation_map {
instruction_handler_t lookup_1x(H8Board* h8300h);
instruction_handler_t lookup_10(H8Board* h8300h);
instruction_handler_t lookup_11(H8Board* h8300h);
instruction_handler_t lookup_12(H8Board* h8300h);
instruction_handler_t lookup_17(H8Board* h8300h);
instruction_handler_t lookup_1A(H8Board* h8300h);
instruction_handler_t lookup_1B(H8Board* h8300h);
instruction_handler_t lookup_1F(H8Board* h8300h);
}

namespace operation_map2 {
instruction_parser_t lookup_1x(H8Board* h8300h);
instruction_parser_t lookup_10(H8Board* h8300h);
instruction_parser_t lookup_11(H8Board* h8300h);
instruction_parser_t lookup_12(H8Board* h8300h);
instruction_parser_t lookup_13(H8Board* h8300h);
instruction_parser_t lookup_17(H8Board* h8300h);
instruction_parser_t lookup_1A(H8Board* h8300h);
instruction_parser_t lookup_1B(H8Board* h8300h);
instruction_parser_t lookup_1F(H8Board* h8300h);
}

#endif
