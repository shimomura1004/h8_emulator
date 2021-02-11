#ifndef _INSTRUCTIONS_XOR_INCLUDED_
#define _INSTRUCTIONS_XOR_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace xorl {

int xor_immediate_b(H8300H* h8);
int xor_register_direct_b(H8300H* h8);
int xor_register_direct_l(H8300H* h8);

}
}

#endif
