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

    if (cond()) {
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

    if (cond()) {
        h8->cpu.pc() += instruction.op1.get_immediate16();
    }

    return 0;
}


int h8instructions::bcc::bge_8(H8Board *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (!n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bge_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (!n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::blt_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::blt_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bgt_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (!(h8->cpu.ccr().z() || n_xor_v)) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bgt_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (!(h8->cpu.ccr().z() || n_xor_v)) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::ble_8(H8Board *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (h8->cpu.ccr().z() || n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::ble_16(H8Board *h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    bool n_xor_v = (h8->cpu.ccr().n() && !h8->cpu.ccr().v()) || (!h8->cpu.ccr().n() && h8->cpu.ccr().v());
    if (h8->cpu.ccr().z() || n_xor_v) {
        h8->cpu.pc() += disp;
    }

    return 0;
}


namespace h8instructions {
namespace bcc {

void bra_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bra_8_parse, bra_8_run>(h8, instruction, "bra");
}

int bra_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return true; });
}

void bra_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bra_16_parse, bra_16_run>(h8, instruction, "bra");
}

int bra_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, [=](){ return true; });
}

void bhi_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bhi_8_parse, bhi_8_run>(h8, instruction, "bhi");
}

int bhi_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return !(h8->cpu.ccr().c() || h8->cpu.ccr().z()); });
}

void bhi_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bhi_16_parse, bhi_16_run>(h8, instruction, "bhi");
}

int bhi_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, [=](){ return !(h8->cpu.ccr().c() || h8->cpu.ccr().z()); });
}

void bls_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bls_8_parse, bls_8_run>(h8, instruction, "bls");
}

int bls_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return h8->cpu.ccr().c() || h8->cpu.ccr().z(); });
}

void bls_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bls_16_parse, bls_16_run>(h8, instruction, "bls");
}

int bls_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, [=](){ return h8->cpu.ccr().c() || h8->cpu.ccr().z(); });
}

void bcc_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bcc_8_parse, bcc_8_run>(h8, instruction, "bcc");
}

int bcc_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return !h8->cpu.ccr().c(); });
}

void bcs_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bcs_8_parse, bcs_8_run>(h8, instruction, "bcs");
}

int bcs_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return h8->cpu.ccr().c(); });
}

void bne_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<bne_8_parse, bne_8_run>(h8, instruction, "bne");
}

int bne_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return !h8->cpu.ccr().z(); });
}

void bne_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<bne_16_parse, bne_16_run>(h8, instruction, "bne");
}

int bne_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, [=](){ return !h8->cpu.ccr().z(); });
}

void beq_8_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_8<beq_8_parse, beq_8_run>(h8, instruction, "beq");
}

int beq_8_run(H8Board* h8, Instruction& instruction)
{
    return run_template_8(h8, instruction, [=](){ return h8->cpu.ccr().z(); });
}

void beq_16_parse(H8Board* h8, Instruction& instruction)
{
    parse_template_16<beq_16_parse, beq_16_run>(h8, instruction, "beq");
}

int beq_16_run(H8Board* h8, Instruction& instruction)
{
    return run_template_16(h8, instruction, [=](){ return h8->cpu.ccr().z(); });
}


}
}
