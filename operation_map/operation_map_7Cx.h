#ifndef _OPERATION_MAP_7C_INCLUDED_
#define _OPERATION_MAP_7C_INCLUDED_

#include "operation_map.h"

namespace operation_map {
instruction_handler_t lookup_7Cr0xx(H8300H* h8300h);
instruction_handler_t lookup_7Cr074x(H8300H* h8300h);
instruction_handler_t lookup_7Cr075x(H8300H* h8300h);
instruction_handler_t lookup_7Cr076x(H8300H* h8300h);
instruction_handler_t lookup_7Cr077x(H8300H* h8300h);
}

#endif
