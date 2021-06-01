#ifndef _INSTRUCTION_ARITHMETIC_HELPER_INCLUDED_
#define _INSTRUCTION_ARITHMETIC_HELPER_INCLUDED_

#include "instruction.h"

// 算術演算命令(and, or, ...)のテンプレート

namespace h8instructions {
namespace arithmetic {

template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_immediate_b(H8Board* h8, Instruction& instruction, const char* name)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    instruction.name = name;
    instruction.op1.set_immediate8(h8->fetch_instruction_byte(1));
    instruction.op2.set_register_direct8(b0 & 0x0f);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_immediate_b(H8Board* h8, Instruction& instruction, F op)
{
    Register8& reg = h8->cpu.reg8(instruction.op2.get_register_direct8());
    uint8_t imm = instruction.op1.get_immediate8();

    uint8_t value = op(reg.get(), imm);
    reg.set(value);
    h8instructions::update_ccr_nzv<int8_t>(h8, value);
    h8->cpu.pc() += 2;

    return 0;
}


template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_immediate_w(H8Board *h8, Instruction& instruction, const char* name)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint16_t immediate = h8instructions::parse_immediate<uint16_t>(h8, 2);

    instruction.name = name;
    instruction.op1.set_immediate16(immediate);
    instruction.op2.set_register_direct16(b1 & 0x0f);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_immediate_w(H8Board *h8, Instruction& instruction, F op)
{
    Register16& reg = h8->cpu.reg16(instruction.op2.get_register_direct16());
    uint16_t imm = instruction.op1.get_immediate16();

    uint16_t value = op(reg.get(), imm);
    reg.set(value);
    h8instructions::update_ccr_nzv<uint16_t>(h8, value);
    h8->cpu.pc() += 4;

    return 0;
}

template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_immediate_l(H8Board *h8, Instruction& instruction, const char* name)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);
    uint32_t immediate = h8instructions::parse_immediate<uint32_t>(h8, 2);

    instruction.name = name;
    instruction.op1.set_immediate32(immediate);
    instruction.op2.set_register_direct32(b1 & 0x07);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_immediate_l(H8Board *h8, Instruction& instruction, F op)
{
    Register32& reg = h8->cpu.reg32(instruction.op2.get_register_direct32());
    uint32_t imm = instruction.op1.get_immediate32();

    uint32_t value = op(reg.get(), imm);
    reg.set(value);
    h8instructions::update_ccr_nzv<int32_t>(h8, value);
    h8->cpu.pc() += 6;

    return 0;
}

template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_register_direct_b(H8Board* h8, Instruction& instruction, const char* name)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = name;
    instruction.op1.set_register_direct8((b1 & 0xf0) >> 4);
    instruction.op2.set_register_direct8(b1 & 0x0f);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_register_direct_b(H8Board* h8, Instruction& instruction, F op)
{
    const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
    Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

    uint8_t src_value = src.get();
    uint8_t dst_value = dst.get();
    uint8_t result_value = op(src_value, dst_value);

    dst.set(result_value);
    h8instructions::update_ccr_nzv<int8_t>(h8, (int8_t)result_value);
    h8->cpu.pc() += 2;

    return 0;
}


template<instruction_parser_t parser, instruction_runner_t runner>
inline
void parse_register_direct_l(H8Board* h8, Instruction& instruction, const char* name)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);

    instruction.name = name;
    instruction.op1.set_register_direct32((b3 & 0x70) >> 4);
    instruction.op2.set_register_direct32(b3 & 0x07);

    instruction.parser = parser;
    instruction.runner = runner;
}

template<class F>
inline
int run_register_direct_l(H8Board* h8, Instruction& instruction, F op)
{
    const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
    Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

    uint32_t src_value = src.get();
    uint32_t dst_value = dst.get();
    uint32_t result_value = op(src_value, dst_value);

    dst.set(result_value);
    h8instructions::update_ccr_nzv<int32_t>(h8, (int32_t)result_value);
    h8->cpu.pc() += 4;

    return 0;
}

}
}

#endif
