#ifndef _INSTRUCTIONS_BSET_INCLUDED_
#define _INSTRUCTIONS_BSET_INCLUDED_

#include "board/h8board.h"

namespace h8instructions {
namespace bset {

int bset_register_direct_imm(H8Board* h8);
int bset_register_indirect(H8Board* h8);

}
}

#endif
