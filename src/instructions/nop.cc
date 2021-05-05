#include "nop.h"

int h8instructions::nop::nop(H8Board* h8)
{
    h8->cpu.pc() += 2;
    return 0;
}
