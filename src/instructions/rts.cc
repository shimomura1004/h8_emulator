#include "rts.h"

int h8instructions::rts::rts(H8300H *h8)
{
    h8->cpu.pc() = h8->pop_from_stack_l();
    return 0;
}
