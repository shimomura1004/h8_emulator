#include "andc.h"

namespace h8instructions {
namespace andc {

void andc_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "andc";
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_ccr();

    instruction.parser = andc_parse;
    instruction.runner = andc_run;
}

// TODO: ccr.raw/ccr.set を使う
int andc_run(H8Board* h8, Instruction& instruction)
{
    uint8_t imm = instruction.op1.get_immediate8();

    (h8->cpu.ccr().i()  && (imm & (1<<7))) ? h8->cpu.ccr().set_i()  : h8->cpu.ccr().clear_i();
    (h8->cpu.ccr().ui() && (imm & (1<<6))) ? h8->cpu.ccr().set_ui() : h8->cpu.ccr().clear_ui();
    (h8->cpu.ccr().h()  && (imm & (1<<5))) ? h8->cpu.ccr().set_h()  : h8->cpu.ccr().clear_h();
    (h8->cpu.ccr().u()  && (imm & (1<<4))) ? h8->cpu.ccr().set_u()  : h8->cpu.ccr().clear_u();
    (h8->cpu.ccr().n()  && (imm & (1<<3))) ? h8->cpu.ccr().set_n()  : h8->cpu.ccr().clear_n();
    (h8->cpu.ccr().z()  && (imm & (1<<2))) ? h8->cpu.ccr().set_z()  : h8->cpu.ccr().clear_z();
    (h8->cpu.ccr().v()  && (imm & (1<<1))) ? h8->cpu.ccr().set_v()  : h8->cpu.ccr().clear_v();
    (h8->cpu.ccr().c()  && (imm & (1<<0))) ? h8->cpu.ccr().set_c()  : h8->cpu.ccr().clear_c();

    h8->cpu.pc() += 2;

    return 0;
}

}
}
