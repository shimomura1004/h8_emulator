#ifndef _INSTRUCTIONS_MOV_INCLUDED_
#define _INSTRUCTIONS_MOV_INCLUDED_

#include "h8board.h"

namespace h8instructions {
namespace mov {

template<class T>
void update_ccr(H8Board* h8300h, T value) {
    CCR& ccr = h8300h->cpu.ccr();

    (value < 0)  ? ccr.set_n() : ccr.clear_n();
    (value == 0) ? ccr.set_z() : ccr.clear_z();
    ccr.clear_v();
}

}
}

#endif
