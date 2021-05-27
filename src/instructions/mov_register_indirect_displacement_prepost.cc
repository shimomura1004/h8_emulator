#include "mov_register_indirect_displacement_prepost.h"

namespace h8instructions {
namespace mov {

void register_indirect_with_increment_decrement_b_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    instruction.name = "mov.b";
    instruction.parser = h8instructions::mov::register_indirect_with_increment_decrement_b_parser;
    instruction.runner = h8instructions::mov::register_indirect_with_increment_decrement_b_run;

    if ((b1 & 0x80) == 0) {
        // mov.b @ERs+,Rd
        instruction.op1.set_register_indirect_with_post_incement((b1 & 0x70) >> 4);
        instruction.op2.set_register_direct8(b1 & 0x0f);
    } else {
        // mov.b Rs,@-ERd
        instruction.op1.set_register_direct8(b1 & 0x0f);
        instruction.op2.set_register_indirect_with_pre_decrement((b1 & 0x70) >> 4);
    }
}

int register_indirect_with_increment_decrement_b_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterIndirectWithPostIncement: {
        // mov.b @ERs+,Rd
        uint8_t src_register_index = instruction.op1.get_register_indirect_with_post_incement();
        Register8& dst = h8->cpu.reg8(instruction.op2.get_register_direct8());

        uint8_t value = h8->pop_from_stack_b(src_register_index);
        dst.set(value);

        h8instructions::update_ccr<int8_t>(h8, value);

        h8->cpu.pc() += 2;

        return 0;
    }
    case addressing_mode_t::RegisterDirect8: {
        // mov.b Rs,@-ERd
        const Register8& src = h8->cpu.reg8(instruction.op1.get_register_direct8());
        uint8_t dst_register_index = instruction.op2.get_register_indirect_with_pre_decrement();

        uint8_t value = src.get();
        h8->push_to_stack_b(value, dst_register_index);

        h8instructions::update_ccr<int8_t>(h8, value);

        h8->cpu.pc() += 2;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

void register_indirect_with_increment_decrement_l_parser(H8Board* h8, Instruction& instruction)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);

    instruction.name = "mov.l";
    instruction.parser = h8instructions::mov::register_indirect_with_increment_decrement_l_parser;
    instruction.runner = h8instructions::mov::register_indirect_with_increment_decrement_l_run;

    if ((b3 & 0x80) == 0) {
        // mov.l @ERs+,ERd
        instruction.op1.set_register_indirect_with_post_incement((b3 & 0x70) >> 4);
        instruction.op2.set_register_direct32(b3 & 0x07);
    } else {
        // mov.l ERs,@-ERd
        instruction.op1.set_register_direct32(b3 & 0x07);
        instruction.op2.set_register_indirect_with_pre_decrement((b3 & 0x70) >> 4);
    }
}

int register_indirect_with_increment_decrement_l_run(H8Board* h8, Instruction& instruction)
{
    addressing_mode_t mode = instruction.op1.get_mode();

    switch (mode) {
    case addressing_mode_t::RegisterIndirectWithPostIncement: {
        // mov.l @ERs+,ERd
        uint8_t src_register_index = instruction.op1.get_register_indirect_with_post_incement();
        Register32& dst = h8->cpu.reg32(instruction.op2.get_register_direct32());

        uint32_t value = h8->pop_from_stack_l(src_register_index);
        dst.set(value);

        h8instructions::update_ccr<int32_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    case addressing_mode_t::RegisterDirect32: {
        // mov.l ERs,@-ERd
        const Register32& src = h8->cpu.reg32(instruction.op1.get_register_direct32());
        uint8_t dst_register_index = instruction.op2.get_register_indirect_with_pre_decrement();

        uint32_t value = src.get();
        h8->push_to_stack_l(value, dst_register_index);

        h8instructions::update_ccr<int32_t>(h8, value);

        h8->cpu.pc() += 4;

        return 0;
    }
    default:
        fprintf(stderr, "Error: mismatched operator and operands\n");
        return -1;
    }
}

}
}