#ifndef _INSTRUCTIONS_MOV_INCLUDED_
#define _INSTRUCTIONS_MOV_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

template<class T>
void update_ccr(H8300H* h8300h, T value) {
    CCR& ccr = h8300h->ccr;

    (value < 0)  ? ccr.set_n() : ccr.clear_n();
    (value == 0) ? ccr.set_z() : ccr.clear_z();
    ccr.clear_v();
}

// MOV 命令は下位バイトまで読まないと判定ができないため mov 関数内部で分岐させる
int mov(H8300H *h8);

}
}

#endif
