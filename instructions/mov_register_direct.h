#ifndef _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_DIRECT_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

int register_direct_b(H8300H* h8);
int register_direct_w(H8300H* h8);
int register_direct_l(H8300H* h8);

}
}

#endif
