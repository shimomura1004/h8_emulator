#include "rte.h"

int h8instructions::rte::rte(H8300H *h8)
{
    h8->restore_pc_and_ccr_from_stack();
    return 0;
}
