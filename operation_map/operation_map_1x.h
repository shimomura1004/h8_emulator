#ifndef _OPERATION_MAP_1_INCLUDED_
#define _OPERATION_MAP_1_INCLUDED_

#include "operation_map.h"

namespace operation_map {
instruction_handler_t lookup_1x(H8300H* h8300h);
instruction_handler_t lookup_10(H8300H* h8300h);
instruction_handler_t lookup_11(H8300H* h8300h);
instruction_handler_t lookup_12(H8300H* h8300h);
instruction_handler_t lookup_13(H8300H* h8300h);
instruction_handler_t lookup_17(H8300H* h8300h);
instruction_handler_t lookup_1A(H8300H* h8300h);
instruction_handler_t lookup_1B(H8300H* h8300h);
instruction_handler_t lookup_1F(H8300H* h8300h);
}

#endif
