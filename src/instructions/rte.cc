#include "rte.h"

namespace h8instructions {
namespace rte {

void rte_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "rte";
    instruction.op1.set_not_used();
    instruction.op2.set_not_used();

    instruction.parser = rte_parse;
    instruction.runner = rte_run;
}

int rte_run(H8Board* h8, Instruction& instruction)
{
    h8->restore_pc_and_ccr_from_stack();
    return 0;
}

}
}
