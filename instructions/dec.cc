#include "dec.h"

template<int n, class T>
static void update_ccr(H8300H* h8, T dst_value, T result_value)
{
    bool dst_value_nth_bit = dst_value & (1 << (n - 1));
    bool result_value_nth_bit = result_value & (1 << (n - 1));

    (result_value < 0) ? h8->ccr.set_n() : h8->ccr.clear_n();
    (result_value == 0) ? h8->ccr.set_z() : h8->ccr.clear_z();

    bool v = dst_value_nth_bit && !result_value_nth_bit;
    v ? h8->ccr.set_v() : h8->ccr.clear_v();
}

int h8instructions::dec::dec_b(H8300H *h8)
{
    return -1;
}

int h8instructions::dec::dec_w(H8300H *h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint8_t b1h = (b1 & 0xf0) >> 4;
    uint8_t register_index = b1 & 0x0f;
    Register32& reg = h8->reg[register_index % 8];

    int16_t dst_value = (register_index < 8) ? reg.get_r() : reg.get_e();
    int16_t result_value = dst_value;
    if (b1h == 0x05) {
        result_value -= 1;
    } else if (b1h == 0x0d) {
        result_value -= 2;
    } else {
        fprintf(stderr, "Unknown dec.w format\n");
        return -1;
    }
    (register_index < 8) ? reg.set_r(result_value) : reg.set_e(result_value);

    update_ccr<16, int16_t>(h8, dst_value, result_value);

    h8->pc += 2;

    return 0;
}

int h8instructions::dec::dec_l(H8300H *h8)
{
    return -1;
}