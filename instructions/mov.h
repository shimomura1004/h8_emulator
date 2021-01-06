#ifndef _INSTRUCTIONS_MOV_INCLUDED_
#define _INSTRUCTIONS_MOV_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace mov {

int reg_to_reg(H8300H* h8);
int location_to_reg(H8300H* h8);
int reg_to_location(H8300H* h8);

}
}

#endif
