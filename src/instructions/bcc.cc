#include "bcc.h"

template<instruction_parser_t parser, instruction_runner_t runner>
static inline
void parse_template_8(H8Board* h8, Instruction& instruction, const char* name)
{
    instruction.name = name;
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_not_used();

    instruction.parser = parser;
    instruction.runner = runner;
}

template<typename F>
static inline
int run_template_8(H8Board* h8, Instruction& instruction, F cond)
{
    h8->cpu.pc() += 2;

    if (cond(h8)) {
        h8->cpu.pc() += instruction.op1.get_immediate8();
    }

    return 0;
}

template<instruction_parser_t parser, instruction_runner_t runner>
inline void parse_template_16(H8Board* h8, Instruction& instruction, const char* name)
{
    int16_t displacement = h8instructions::parse_immediate<int16_t>(h8, 2);

    instruction.name = name;
    instruction.op1.set_immediate16(displacement);
    instruction.op2.set_not_used();

    instruction.parser = parser;
    instruction.runner = runner;
}

template<typename F>
inline int run_template_16(H8Board* h8, Instruction& instruction, F cond)
{
    h8->cpu.pc() += 4;

    if (cond(h8)) {
        h8->cpu.pc() += instruction.op1.get_immediate16();
    }

    return 0;
}

#define define_bcc_instruction(name) void name##_8_parse(H8Board* h8, Instruction& instruction) \
{ \
    parse_template_8<name##_8_parse, name##_8_run>(h8, instruction, #name); \
} \
int name##_8_run(H8Board* h8, Instruction& instruction) \
{ \
    return run_template_8(h8, instruction, name##_cond); \
} \
void name##_16_parse(H8Board* h8, Instruction& instruction) \
{ \
    parse_template_16<name##_16_parse, name##_16_run>(h8, instruction, #name); \
} \
int name##_16_run(H8Board* h8, Instruction& instruction) \
{ \
    return run_template_16(h8, instruction, name##_cond); \
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
