#include "bcc.h"

#define define_bcc_instruction(instruction_name) void instruction_name##_8_parse(H8Board* h8, Instruction& instruction) \
{ \
    instruction.name = #instruction_name; \
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1)); \
    instruction.op2.set_not_used(); \
 \
    instruction.parser = instruction_name##_8_parse; \
    instruction.runner = instruction_name##_8_run; \
} \
int instruction_name##_8_run(H8Board* h8, Instruction& instruction) \
{ \
    h8->cpu.pc() += 2; \
 \
    if (instruction_name##_cond(h8)) { \
        h8->cpu.pc() += instruction.op1.get_immediate8(); \
    } \
 \
    return 0; \
} \
void instruction_name##_16_parse(H8Board* h8, Instruction& instruction) \
{ \
    int16_t displacement = h8instructions::parse_immediate<int16_t>(h8, 2); \
 \
    instruction.name = #instruction_name; \
    instruction.op1.set_immediate16(displacement); \
    instruction.op2.set_not_used(); \
 \
    instruction.parser = instruction_name##_16_parse; \
    instruction.runner = instruction_name##_16_run; \
} \
int instruction_name##_16_run(H8Board* h8, Instruction& instruction) \
{ \
    h8->cpu.pc() += 4; \
 \
    if (instruction_name##_cond(h8)) { \
        h8->cpu.pc() += instruction.op1.get_immediate16(); \
    } \
 \
    return 0; \
}

namespace h8instructions {
namespace bcc {

static inline bool bra_cond(H8Board *h8)
{
    return true;
}
define_bcc_instruction(bra)

static inline bool bhi_cond(H8Board *h8)
{
    return !(h8->cpu.ccr().c() || h8->cpu.ccr().z());
}
define_bcc_instruction(bhi)

static inline bool bls_cond(H8Board *h8)
{
    return h8->cpu.ccr().c() || h8->cpu.ccr().z();
}
define_bcc_instruction(bls)

static inline bool bcc_cond(H8Board *h8)
{
    return !h8->cpu.ccr().c();
}
define_bcc_instruction(bcc)

static inline bool bcs_cond(H8Board *h8)
{
    return h8->cpu.ccr().c();
}
define_bcc_instruction(bcs)

static inline bool bne_cond(H8Board *h8)
{
    return !h8->cpu.ccr().z();
}
define_bcc_instruction(bne)

static inline bool beq_cond(H8Board *h8) {
    return h8->cpu.ccr().z();
}
define_bcc_instruction(beq)

static inline bool bge_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return !n_xor_v;
}
define_bcc_instruction(bge)

static inline bool blt_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return n_xor_v;
}
define_bcc_instruction(blt)

static inline bool bgt_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return !(h8->cpu.ccr().z() || n_xor_v);
}
define_bcc_instruction(bgt)

static inline bool ble_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return h8->cpu.ccr().z() || n_xor_v;
}
define_bcc_instruction(ble)

}
}
