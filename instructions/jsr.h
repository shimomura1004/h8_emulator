#ifndef _INSTRUCTIONS_JSR_INCLUDED_
#define _INSTRUCTIONS_JSR_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace jsr {

int jsr_register_indirect(H8300H* h8);
int jsr_absolute_address(H8300H* h8);

}
}

#endif