#include "sleep.h"

namespace h8instructions {
namespace sleep {

void sleep_parse(H8Board* h8, Instruction& instruction)
{
    instruction.name = "sleep";
    instruction.op1.set_not_used();
    instruction.op2.set_not_used();

    instruction.parser = sleep_parse;
    instruction.runner = sleep_run;
}
int sleep_run(H8Board* h8, Instruction& instruction)
{
    h8->is_sleep = true;
    h8->cpu.pc() += 2;
    return 0;
}

}
}
