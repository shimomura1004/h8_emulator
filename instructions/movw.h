#ifndef _INSTRUCTIONS_MOVW_INCLUDED_
#define _INSTRUCTIONS_MOVW_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace movw {
namespace immediate {
const unsigned char FIRST_BYTE = 0x79;
int execute(H8300H* h8300h);
}

// namespace regs {
// const unsigned char FIRST_BYTE = 0x0f;
// int execute(H8300H* h8300h);
// }

// const unsigned char FIRST_BYTE = 0x01;
// int execute(H8300H* h8300h);

}
}

#endif