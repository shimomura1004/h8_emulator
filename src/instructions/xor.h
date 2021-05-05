#ifndef _INSTRUCTIONS_XOR_INCLUDED_
#define _INSTRUCTIONS_XOR_INCLUDED_

#include "h8board.h"

namespace h8instructions {
namespace xorl {

int xor_immediate_b(H8Board* h8);
int xor_register_direct_b(H8Board* h8);
int xor_register_direct_l(H8Board* h8);

}
}

#endif
