#include "cmp.h"
#include "sub.h"

static int immediate_b(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);
    uint8_t register_index = b0 & 0x0f;
    Register32& reg = h8->reg[register_index % 8];

    int8_t src_value = h8->fetch_instruction_byte(1);
    int8_t dst_value = (register_index < 8) ? reg.get_rh() : reg.get_rl();
    int8_t result_value = dst_value - src_value;

    h8instructions::sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    h8->pc += 2;

    return 0;
}

static int register_indirect_b(H8300H* h8)
{
    return -1;
}

int h8instructions::cmp::cmp_b(H8300H *h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    if (((b0 & 0xf0) >> 4) == 0x0A) {
        return immediate_b(h8);
    } else if (b0 == 0x1c) {
        return register_indirect_b(h8);
    } else {
        return -1;
    }
}
