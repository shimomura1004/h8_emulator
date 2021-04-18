#ifndef _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_INCLUDED_
#define _INSTRUCTIONS_MOV_REGISTER_INDIRECT_DISPACEMENT_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_displacement16_b(H8300H* h8);
int register_indirect_with_displacement16_w(H8300H* h8);
int register_indirect_with_displacement16_l(H8300H* h8);

int register_indirect_with_displacement24_b(H8300H* h8);
// int register_indirect_with_displacement24_w(H8300H* h8);
int register_indirect_with_displacement24_l(H8300H* h8);

int register_indirect_with_increment_decrement_b(H8300H* h8);
int register_indirect_with_increment_decrement_l(H8300H* h8);

}
}

#endif
