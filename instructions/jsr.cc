#include "jsr.h"

int h8instructions::jsr::execute(H8300H* h8300h)
{
    unsigned char abs_[4];
    abs_[3] = 0;
    abs_[2] = h8300h->fetch_instruction_byte(1);
    abs_[1] = h8300h->fetch_instruction_byte(2);
    abs_[0] = h8300h->fetch_instruction_byte(3);
    int32_t abs = *(int32_t*)abs_;

    // PC をスタックに退避したあと PC を更新
    h8300h->save_pc_and_ccr_to_stack();
    h8300h->pc = abs;

    return 0;
}
