#ifndef _INSTRUCTIONS_MOV_IMMEDIATE_INCLUDED_
#define _INSTRUCTIONS_MOV_IMMEDIATE_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

int immediate_b(H8300H* h8);
int immediate_w(H8300H* h8);
int immediate_l(H8300H* h8);

}
}

#endif
