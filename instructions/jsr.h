#ifndef _INSTRUCTIONS_JSR_INCLUDED_
#define _INSTRUCTIONS_JSR_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace jsr {

const unsigned char FIRST_BYTE = 0x5e;
int execute(H8300H* h8300h);

}
}

#endif