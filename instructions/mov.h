#ifndef _INSTRUCTIONS_MOV_INCLUDED_
#define _INSTRUCTIONS_MOV_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace mov {

// MOV 命令は下位バイトまで読まないと判定ができないため mov 関数内部で分岐させる
int mov(H8300H *h8);

}
}

#endif
