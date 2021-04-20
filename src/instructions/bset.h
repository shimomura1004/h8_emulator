#ifndef _INSTRUCTIONS_BSET_INCLUDED_
#define _INSTRUCTIONS_BSET_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace bset {

int bset_register_direct_imm(H8300H* h8);
int bset_register_indirect(H8300H* h8);

}
}

#endif
