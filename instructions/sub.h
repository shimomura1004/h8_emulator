#ifndef _INSTRUCTIONS_SUB_INCLUDED_
#define _INSTRUCTIONS_SUB_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace sub {

int sub_b(H8300H *h8);
int sub_w(H8300H *h8);
int sub_l(H8300H *h8);
int sub_immediate_l(H8300H *h8);

}
}

#endif
