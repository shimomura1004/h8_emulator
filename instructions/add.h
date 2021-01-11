#ifndef _INSTRUCTIONS_ADD_INCLUDED_
#define _INSTRUCTIONS_ADD_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace add {

int add_immediate_b(H8300H* h8);
int add_register_direct_b(H8300H* h8);
int add_immediate_l(H8300H* h8);
int add_register_direct_l(H8300H* h8);

}
}

#endif
