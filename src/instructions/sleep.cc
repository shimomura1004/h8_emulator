#include "sleep.h"

int h8instructions::sleep::sleep(H8Board *h8)
{
    h8->is_sleep = true;
    h8->cpu.pc() += 2;

    return 0;
}
