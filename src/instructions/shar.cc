#include "shar.h"

namespace h8instructions {
namespace shar {

#define define_shar(opsize, opsize_char) \
void shar_##opsize_char##_parse(H8Board* h8, Instruction& instruction) \
{ \
    uint8_t b1 = h8->fetch_instruction_byte(1); \
 \
    instruction.name = "shar." # opsize_char; \
    instruction.op1.set_register_direct##opsize(b1 & 0x0f); \
    instruction.op2.set_not_used(); \
 \
    instruction.parser = shar_##opsize_char##_parse; \
    instruction.runner = shar_##opsize_char##_run; \
} \
 \
int shar_##opsize_char##_run(H8Board* h8, Instruction& instruction) \
{ \
    static const int##opsize##_t mask = (int##opsize##_t)(1 << (opsize - 1)); \
 \
    Register##opsize& reg = h8->cpu.reg##opsize(instruction.op1.get_register_direct##opsize()); \
 \
    int##opsize##_t value = reg.get(); \
    bool msb = value & mask; \
    bool lsb = value & 0x01; \
    /* gcc では signed の場合は算術シフトされるが、 \
       他の環境を考慮し念のため最上位ビットを元の値にしておく */ \
    value = (value >> 1) | (msb ? mask : 0); \
 \
    reg.set(value); \
 \
    (value < 0) ? h8->cpu.ccr().set_n() : h8->cpu.ccr().clear_n(); \
    (value == 0) ? h8->cpu.ccr().set_z() : h8->cpu.ccr().clear_z(); \
    h8->cpu.ccr().clear_v(); \
    lsb ? h8->cpu.ccr().set_c() : h8->cpu.ccr().clear_c(); \
 \
    h8->cpu.pc() += 2; \
 \
    return 0; \
}

define_shar(16, w);
define_shar(32, l);

}
}
