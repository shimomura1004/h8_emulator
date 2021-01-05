#include "h8300h.h"
#include <stdio.h>

int main (int argc, char* argv[])
{
    H8300H h8;
    Register32 reg;

    reg.set_er(0x12345678);
    printf("0x%08x\n", reg.get_er());

    printf("0x%08x\n", reg.get_e());
    reg.set_e(0xabcd);
    printf("0x%08x\n", reg.get_e());

    printf("0x%08x\n", reg.get_r());
    reg.set_r(0x9876);
    printf("0x%08x\n", reg.get_r());

    printf("0x%08x 0x%08x\n", reg.get_rh(), reg.get_rl());
    reg.set_rh(0x22);
    reg.set_rl(0x44);
    printf("0x%08x 0x%08x\n", reg.get_rh(), reg.get_rl());

    printf("0x%08x\n", reg.get_er());

    return 0;
}
