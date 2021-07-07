#include "and.h"
#include "arithmetic_helper.h"

namespace h8instructions {
namespace andl {

void immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_immediate_b<immediate_b_parse, immediate_b_run>(h8, instruction, "and.b");
}

int immediate_b_run(H8Board *h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_b(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 & op2;});
}

void immediate_w_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_immediate_w<immediate_w_parse, immediate_w_run>(h8, instruction, "and.w");
}

int immediate_w_run(H8Board *h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_w(h8, instruction, [](uint16_t op1, uint16_t op2){return op1 & op2;});
}

void immediate_l_parse(H8Board *h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_immediate_l<immediate_l_parse, immediate_l_run>(h8, instruction, "and.l");
}

int immediate_l_run(H8Board *h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_l(h8, instruction, [](uint32_t op1, uint32_t op2){return op1 & op2;});
}

}
}
