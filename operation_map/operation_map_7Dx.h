#ifndef _OPERATION_MAP_7D_INCLUDED_
#define _OPERATION_MAP_7D_INCLUDED_

#include "operation_map.h"

namespace operation_map {
instruction_handler_t lookup_7Dr0xx(H8300H* h8300h);
instruction_handler_t lookup_7Dr067x(H8300H* h8300h);
}

#endif
