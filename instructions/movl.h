#ifndef _INSTRUCTIONS_MOVL_INCLUDED_
#define _INSTRUCTIONS_MOVL_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace movl {
namespace immediate {
const unsigned char FIRST_BYTE = 0x7a;
int execute(H8300H* h8300h);
}

namespace regs {
const unsigned char FIRST_BYTE = 0x0f;
int execute(H8300H* h8300h);
}

const unsigned char FIRST_BYTE = 0x01;
int execute(H8300H* h8300h);

}
}

#endif