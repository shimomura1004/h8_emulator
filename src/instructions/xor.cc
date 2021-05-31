#include "xor.h"
#include "arithmetic_helper.h"

namespace h8instructions {
namespace xorl {

void immediate_b_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_immediate_b<immediate_b_parse, immediate_b_run>(h8, instruction, "xor.b");
}

int immediate_b_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_immediate_b(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 ^ op2;});
}

void register_direct_b_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_register_direct_b<register_direct_b_parse, register_direct_b_run>(h8, instruction, "xor.b");
}

int register_direct_b_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_register_direct_b(h8, instruction, [](uint8_t op1, uint8_t op2){return op1 ^ op2;});
}

void register_direct_l_parse(H8Board* h8, Instruction& instruction)
{
    h8instructions::arithmetic::parse_register_direct_l<register_direct_l_parse, register_direct_l_run>(h8, instruction, "xor.l");
}

int register_direct_l_run(H8Board* h8, Instruction& instruction)
{
    return h8instructions::arithmetic::run_register_direct_l(h8, instruction, [](uint32_t op1, uint32_t op2){return op1 ^ op2;});
}

}
}
