#include "shll.h"

namespace h8instructions {
namespace shll {

#define define_shll(opsize, opsize_char) \
void shll_##opsize_char##_parse(H8Board* h8, Instruction& instruction) \
{ \
    uint8_t b1 = h8->fetch_instruction_byte(1); \
 \
    instruction.name = "shll." # opsize_char; \
    instruction.op1.set_register_direct##opsize(b1 & 0x0f); \
    instruction.op2.set_not_used(); \
 \
    instruction.parser = shll_##opsize_char##_parse; \
    instruction.runner = shll_##opsize_char##_run; \
} \
 \
int shll_##opsize_char##_run(H8Board* h8, Instruction& instruction) \
{ \
    static const uint##opsize##_t mask = (1 << (opsize - 1)); \
 \
    Register##opsize& reg = h8->cpu.reg##opsize(instruction.op1.get_register_direct##opsize()); \
 \
    uint##opsize##_t er = reg.get(); \
    bool er_msb = er & mask; \
    er = er << 1; \
 \
    reg.set(er); \
 \
    (er & mask) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n(); \
    (er == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z(); \
    h8->cpu.ccr().clear_v(); \
    er_msb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c(); \
 \
    h8->cpu.pc() += 2; \
 \
    return 0; \
}

define_shll(8, b);
define_shll(16, w);
define_shll(32, l);

}
}
