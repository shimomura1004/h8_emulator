#include "mov_register_indirect_displacement_prepost.h"
#include "mov.h"

namespace h8instructions {
namespace mov {

int register_indirect_with_increment_decrement_b(H8300H* h8)
{
    uint8_t b1 = h8->fetch_instruction_byte(1);

    if ((b1 & 0x80) == 0) {
        uint8_t src_register_index = (b1 & 0x70) >> 4;
        uint8_t dst_register_index = (b1 & 0x0f);
        Register8& dst = h8->reg8[dst_register_index];

        uint8_t value = h8->pop_from_stack_b(src_register_index);
        dst.set(value);

        h8instructions::mov::update_ccr<int8_t>(h8, value);
        h8->pc += 2;
    } else {
        return -1;
    }

    return 0;
}

int register_indirect_with_displacement24_w(H8300H* h8)
{
    return -1;
}

int register_indirect_with_increment_decrement_l(H8300H* h8)
{
    uint8_t b3 = h8->fetch_instruction_byte(3);
    if ((b3 & 0x80) == 0) {
        uint8_t src_register_index = (b3 & 0x70) >> 4;
        uint8_t dst_register_index = (b3 & 0x07);
        Register32& dst = h8->reg[dst_register_index];

        uint32_t value = h8->pop_from_stack_l(src_register_index);
        dst.set(value);

        h8instructions::mov::update_ccr<int32_t>(h8, value);
    } else {
        uint8_t src_register_index = (b3 & 0x07);
        uint8_t dst_register_index = (b3 & 0x70) >> 4;
        Register32& src = h8->reg[src_register_index];

        h8->push_to_stack_l(src.get(), dst_register_index);
        h8instructions::mov::update_ccr<int32_t>(h8, src.get());
    }

    h8->pc += 4;

    return 0;
}

}
}

namespace h8instructions {
namespace mov {

// void register_indirect_with_increment_decrement_b_parser(H8300H* h8, Instruction* instruction);
// int register_indirect_with_increment_decrement_b_run(H8300H* h8, Instruction* instruction);
// void register_indirect_with_increment_decrement_l_parser(H8300H* h8, Instruction* instruction);
// int register_indirect_with_increment_decrement_l_run(H8300H* h8, Instruction* instruction);

}
}