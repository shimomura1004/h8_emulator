#include "nop.h"

namespace h8instructions {
namespace nop {

void nop_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "nop";
    instruction.op1.set_not_used();
    instruction.op2.set_not_used();

    instruction.parser = nop_parse;
    instruction.runner = nop_run;
}

int nop_run(H8Board* h8, Instruction& instruction)
{
    h8->cpu.pc() += 2;
    return 0;
}

}
}
