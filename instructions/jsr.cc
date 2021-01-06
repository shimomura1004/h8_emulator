#include "jsr.h"

int h8instructions::jsr::absolute_address(H8300H* h8)
{
    unsigned char abs_[4];
    abs_[3] = 0;
    abs_[2] = h8->fetch_instruction_byte(1);
    abs_[1] = h8->fetch_instruction_byte(2);
    abs_[0] = h8->fetch_instruction_byte(3);
    int32_t abs = *(int32_t*)abs_;

    h8->push_to_stack_l(h8->pc + 4);
    h8->pc = abs;

    return 0;
}
