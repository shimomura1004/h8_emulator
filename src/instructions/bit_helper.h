#ifndef _INSTRUCTION_BIT_HELPER_INCLUDED_
#define _INSTRUCTION_BIT_HELPER_INCLUDED_

#include "instruction.h"

// ビット操作命令(bset, bclr, btst)のテンプレート

namespace h8instructions {
namespace bit {

template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_register_direct_immediate(H8Board* h8, Instruction& instruction, const char* name)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = name;
    instruction.op1.set_immediate8((b1 & 0x70) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_register_direct_immediate(H8Board* h8, Instruction& instruction, F op)
{
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());
    uint8_t imm = instruction.op1.get_immediate8();

    uint8_t mask = (1 << imm);
    uint8_t value = op(dst.get(), mask);
    dst.set(value);
    h8->cpu.pc() += 2;

    return 0;
}


}
}

#endif
