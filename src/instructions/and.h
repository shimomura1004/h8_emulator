#ifndef _INSTRUCTIONS_AND_INCLUDED_
#define _INSTRUCTIONS_AND_INCLUDED_

#include "board/h8board.h"

namespace h8instructions {
namespace andl {

int and_immediate_b(H8Board *h8);
int and_immediate_w(H8Board *h8);
int and_immediate_l(H8Board *h8);

}
}

#endif
