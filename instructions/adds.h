#ifndef _INSTRUCTIIONS_ADDS_INCLUDED_
#define _INSTRUCTIIONS_ADDS_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace adds {

const unsigned char FIRST_BYTE = 0x0b;

int execute(H8300H* h8300h);

}
}

#endif
