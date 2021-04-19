#ifndef _INSTRUCTIONS_MOV_ABSOLUTE_ADDRESS_INCLUDED_
#define _INSTRUCTIONS_MOV_ABSOLUTE_ADDRESS_INCLUDED_

#include "h8300h.h"

namespace h8instructions {
namespace mov {

int absolute_address_24_b(H8300H* h8);
int absolute_address_24_w(H8300H* h8);
int absolute_address_24_l(H8300H* h8);

}
}

#endif
