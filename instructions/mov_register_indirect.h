#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

int register_indirect_b(H8300H* h8);
int register_indirect_w(H8300H* h8);
int register_indirect_l(H8300H* h8);

}
}

#endif
