#ifndef _OPERATION_MAP_7C_INCLUDED_
#define _OPERATION_MAP_7C_INCLUDED_

#include "operation_map.h"

namespace operation_map2 {
instruction_parser_t lookup_7Cr0xx(H8Board* h8300h);
instruction_parser_t lookup_7Cr074x(H8Board* h8300h);
instruction_parser_t lookup_7Cr075x(H8Board* h8300h);
instruction_parser_t lookup_7Cr076x(H8Board* h8300h);
instruction_parser_t lookup_7Cr077x(H8Board* h8300h);
}

#endif
