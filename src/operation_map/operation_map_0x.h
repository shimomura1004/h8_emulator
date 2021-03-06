#ifndef _OPERATION_MAP_0_INCLUDED_
#define _OPERATION_MAP_0_INCLUDED_

#include "operation_map.h"
#include "instructions/instruction.h"

namespace operation_map2 {
instruction_parser_t lookup_0x(H8Board* h8300h);
instruction_parser_t lookup_01(H8Board* h8300h);
instruction_parser_t lookup_01C05x(H8Board* h8300h);
instruction_parser_t lookup_01D05x(H8Board* h8300h);
instruction_parser_t lookup_01F06x(H8Board* h8300h);
instruction_parser_t lookup_0A(H8Board* h8300h);
instruction_parser_t lookup_0B(H8Board* h8300h);
instruction_parser_t lookup_0F(H8Board* h8300h);
}

#endif
