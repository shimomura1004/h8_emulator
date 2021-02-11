#ifndef _INSTRUCTIONS_CMP_INCLUDED_
#define _INSTRUCTIONS_CMP_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace cmp {

int cmp_immediate_b(H8300H *h8);
int cmp_register_direct_b(H8300H* h8);

int cmp_immediate_w(H8300H* h8);
int cmp_register_direct_w(H8300H* h8);

int cmp_immediate_l(H8300H* h8);
int cmp_register_direct_l(H8300H* h8);

}
}

#endif
