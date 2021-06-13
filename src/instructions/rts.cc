#include "rts.h"

namespace h8instructions {
namespace rts {

void rts_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "rts";
    instruction.op1.set_not_used();
    instruction.op2.set_not_used();

    instruction.parser = rts_parse;
    instruction.runner = rts_run;
}

int rts_run(H8Board* h8, Instruction& instruction)
{
    h8->cpu.pc() = h8->pop_from_stack_l();
    return 0;
}

}
}
