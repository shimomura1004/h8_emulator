#include "shlr.h"

namespace h8instructions {
namespace shlr {

#define define_shlr(opsize, opsize_char) \
void shlr_##opsize_char##_parse(H8Board* h8, Instruction& instruction) \
{ \
    uint8_t b1 = h8->fetch_instruction_byte(1); \
 \
    instruction.name = "shlr." # opsize_char; \
    instruction.op1.set_register_direct##opsize(b1 & 0x0f); \
    instruction.op2.set_not_used(); \
 \
    instruction.parser = shlr_##opsize_char##_parse; \
    instruction.runner = shlr_##opsize_char##_run; \
} \
 \
int shlr_##opsize_char##_run(H8Board* h8, Instruction& instruction) \
{ \
    static const uint##opsize##_t mask = (uint##opsize##_t)~(1 << (opsize - 1)); \
 \
    Register##opsize& reg = h8->cpu.reg##opsize(instruction.op1.get_register_direct##opsize()); \
 \
    uint##opsize##_t er = reg.get(); \
    bool er_lsb = er & 0x01; \
    /* gcc では unsigned の場合は論理シフトされるので不要だが、 \
       他の環境を考慮し念のため最上位ビットを 0 にしておく */ \
    er = (er >> 1) & mask; \
    reg.set(er); \
 \
    h8->cpu.ccr().clear_n(); \
    (er == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z(); \
    h8->cpu.ccr().clear_v(); \
    er_lsb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c(); \
 \
    h8->cpu.pc() += 2; \
 \
    return 0; \
}

define_shlr(8, b);
define_shlr(16, w);
define_shlr(32, l);

}
}