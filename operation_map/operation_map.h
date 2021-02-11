#ifndef _OPERATION_MAP_INCLUDED_
#define _OPERATION_MAP_INCLUDED_

#include "h8300h.h"

typedef int(*instruction_handler_t)(H8300H*);

namespace OperationMap {

instruction_handler_t lookup(H8300H* h8300h);

}

#endif
