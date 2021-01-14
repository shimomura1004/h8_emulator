#include "sleep.h"

int h8instructions::sleep::sleep(H8300H *h8)
{
    h8->is_sleep = true;
    h8->pc += 2;

    return 0;
}
