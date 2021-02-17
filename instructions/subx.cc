#include "subx.h"
#include "sub.h"

int h8instructions::subx::subx_register_direct(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t src_register_index = (b1 & 0xf0) >> 4;
    uint8_t dst_register_index = (b1 & 0x0f);
    const Register8& src = h8->reg8[src_register_index];
    Register8& dst = h8->reg8[dst_register_index];

    char src_value = src.get();
    char dst_value = dst.get();
    char result_value = dst_value - src_value - (h8->ccr.h() ? 1 : 0);
    dst.set(result_value);

    bool prev_z = h8->ccr.z();
    sub::update_ccr<8, int8_t>(h8, src_value, dst_value, result_value);
    if (result_value == 0) {
        prev_z ? h8->ccr.set_z() : h8->ccr.clear_z();
    }

    h8->pc += 2;

    return 0;
}
