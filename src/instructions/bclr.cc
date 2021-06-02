#include "bclr.h"
#include "bit_helper.h"

namespace h8instructions {
namespace bclr {

void register_direct_immediate_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::bit::parse_register_direct_immediate<register_direct_immediate_parse, register_direct_immediate_run>(h8, instruction, "bclr");
}

int register_direct_immediate_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::bit::run_register_direct_immediate(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 & ~op2;});
}

}
}
