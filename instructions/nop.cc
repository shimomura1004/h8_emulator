#include "nop.h"

int h8instructions::nop::nop(H8300H* h8)
{
    h8->pc += 2;
    return 0;
}
