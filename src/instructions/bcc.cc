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

namespace h8instructions {
namespace bcc {

static inline bool bra_cond(H8Board *h8)
{
    return true;
}

void bra_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bra_8_parse, bra_8_run>(h8, instruction, "bra");
}

int bra_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bra_cond);
}

void bra_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bra_16_parse, bra_16_run>(h8, instruction, "bra");
}

int bra_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bra_cond);
}


static inline bool bhi_cond(H8Board *h8)
{
    return !(h8->cpu.ccr().c() || h8->cpu.ccr().z());
}

void bhi_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bhi_8_parse, bhi_8_run>(h8, instruction, "bhi");
}

int bhi_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bhi_cond);
}

void bhi_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bhi_16_parse, bhi_16_run>(h8, instruction, "bhi");
}

int bhi_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bhi_cond);
}


static inline bool bls_cond(H8Board *h8)
{
    return h8->cpu.ccr().c() || h8->cpu.ccr().z();
}

void bls_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bls_8_parse, bls_8_run>(h8, instruction, "bls");
}

int bls_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bls_cond);
}

void bls_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bls_16_parse, bls_16_run>(h8, instruction, "bls");
}

int bls_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bls_cond);
}


static inline bool bcc_cond(H8Board *h8)
{
    return !h8->cpu.ccr().c();
}

void bcc_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bcc_8_parse, bcc_8_run>(h8, instruction, "bcc");
}

int bcc_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bcc_cond);
}


static inline bool bcs_cond(H8Board *h8)
{
    return h8->cpu.ccr().c();
}

void bcs_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bcs_8_parse, bcs_8_run>(h8, instruction, "bcs");
}

int bcs_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bcs_cond);
}


static inline bool bne_cond(H8Board *h8)
{
    return !h8->cpu.ccr().z();
}

void bne_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bne_8_parse, bne_8_run>(h8, instruction, "bne");
}

int bne_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bne_cond);
}

void bne_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bne_16_parse, bne_16_run>(h8, instruction, "bne");
}

int bne_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bne_cond);
}


static inline bool beq_cond(H8Board *h8) {
    return h8->cpu.ccr().z();
}

void beq_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<beq_8_parse, beq_8_run>(h8, instruction, "beq");
}

int beq_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, beq_cond);
}

void beq_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<beq_16_parse, beq_16_run>(h8, instruction, "beq");
}

int beq_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, beq_cond);
}


static inline bool bge_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return !n_xor_v;
}

void bge_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bge_8_parse, bge_8_run>(h8, instruction, "bge");
}

int bge_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bge_cond);
}

void bge_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bge_16_parse, bge_16_run>(h8, instruction, "bge");
}

int bge_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bge_cond);
}


static inline bool blt_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return n_xor_v;
}

void blt_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<blt_8_parse, blt_8_run>(h8, instruction, "blt");
}

int blt_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, blt_cond);
}

void blt_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<blt_16_parse, blt_16_run>(h8, instruction, "blt");
}

int blt_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, blt_cond);
}


static inline bool bgt_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return !(h8->cpu.ccr().z() || n_xor_v);
}

void bgt_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bgt_8_parse, bgt_8_run>(h8, instruction, "bgt");
}

int bgt_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, bgt_cond);
}

void bgt_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bgt_16_parse, bgt_16_run>(h8, instruction, "bgt");
}

int bgt_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, bgt_cond);
}


static inline bool ble_cond(H8Board *h8)
{
    bool n_xor_v = ( h8->cpu.ccr().n() && !h8->cpu.ccr().v())
                || (!h8->cpu.ccr().n() &&  h8->cpu.ccr().v());
    return h8->cpu.ccr().z() || n_xor_v;
}

void ble_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<ble_8_parse, ble_8_run>(h8, instruction, "ble");
}

int ble_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, ble_cond);
}

void ble_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<ble_16_parse, ble_16_run>(h8, instruction, "ble");
}

int ble_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, ble_cond);
}

}
}
