#include "bcc.h"

int h8instructions::bcc::bhi_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);

    h8->cpu.pc() += 2;

    if (!(h8->cpu.ccr().c() || h8->cpu.ccr().z())) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bhi_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    if (!(h8->cpu.ccr().c() || h8->cpu.ccr().z())) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bls_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    if (h8->cpu.ccr().c() || h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bls_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    if (h8->cpu.ccr().c() || h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bcc_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    if (!h8->cpu.ccr().c()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bcs_8(H8Board* h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    if (h8->cpu.ccr().c()) {
        h8->cpu.pc() += disp;
    }

    return 0;}

int h8instructions::bcc::bne_8(H8Board *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    if (!h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::bne_16(H8Board* h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    if (!h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::beq_8(H8Board *h8)
{
    int8_t disp = h8->fetch_instruction_byte(1);
    h8->cpu.pc() += 2;

    if (h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
    }

    return 0;
}

int h8instructions::bcc::beq_16(H8Board *h8)
{
    uint8_t displacement[2];
    displacement[1] = h8->fetch_instruction_byte(2);
    displacement[0] = h8->fetch_instruction_byte(3);
    int16_t disp = *(int16_t*)displacement;

    h8->cpu.pc() += 4;

    if (h8->cpu.ccr().z()) {
        h8->cpu.pc() += disp;
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
    instruction.name = "bra";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));

    instruction.parser = bra_8_parse;
    instruction.runner = bra_8_run;
}

int bra_8_run(H8Board* h8, Instruction& instruction)
{
    h8->cpu.pc() += 2;
    h8->cpu.pc() += instruction.op1.get_immediate8();
    return 0;
}

void bra_16_parse(H8Board* h8, Instruction& instruction)
{
    int16_t displacement = h8instructions::parse_immediate<int16_t>(h8, 2);

    instruction.name = "bra";
    instruction.op1.set_immediate16(displacement);

    instruction.parser = bra_16_parse;
    instruction.runner = bra_16_run;
}

int bra_16_run(H8Board* h8, Instruction& instruction)
{
    h8->cpu.pc() += 4;
    h8->cpu.pc() += instruction.op1.get_immediate16();
    return 0;
}


}
}