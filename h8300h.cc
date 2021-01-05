#include "h8300h.h"
#include "instructions/adds.h"

using namespace h8instructions;

unsigned char H8300H::fetch_instruction_byte(unsigned int offset)
{
    return memory[pc + offset];
}

int H8300H::execute_next_instruction()
{
    unsigned char first_byte = fetch_instruction_byte(0);
    int result = 0;

    switch (first_byte) {
    case adds::FIRST_BYTE:
        result = adds::execute(this);
        break;
    default:
        fprintf(stderr, "Unknown instruction: [0x%02x, ...] at address 0x%08x\n", first_byte, pc);
        result = -1;
        break;
    }

    return result;
}

void H8300H::push_to_stack_w(uint16_t value)
{
    sp.set_er(sp.get_er() - 2);
    unsigned char mem = memory[sp.get_er()];
    *(uint16_t*)&mem = value;
}

uint16_t H8300H::pop_from_stack_w()
{
    unsigned char mem = memory[sp.get_er()];
    unsigned short value = *(uint16_t*)&mem;
    sp.set_er(sp.get_er() + 2);
    return value;
}

void H8300H::push_to_stack_l(uint32_t value)
{
    sp.set_er(sp.get_er() - 4);
    unsigned char mem = memory[sp.get_er()];
    *(uint32_t*)&mem = value;
}

uint32_t H8300H::pop_from_stack_l()
{
    unsigned char mem = memory[sp.get_er()];
    uint32_t value = *(uint32_t*)&mem;
    sp.set_er(sp.get_er() + 4);
    return value;
}

void H8300H::save_pc_and_ccr_to_stack()
{
    // リトルエンディアン環境を想定
    unsigned long ccr_pc;
    ccr_pc = __builtin_bswap32(pc);
    ccr_pc |= ccr.raw() << 24;

    push_to_stack_l(ccr_pc);
}

uint32_t H8300H::load_elf(std::string filepath)
{
    return memory.load_elf(filepath);
}

int H8300H::step()
{
    int result = execute_next_instruction();

    if (result != 0) {
        fprintf(stderr, "Abort.\n");
    }

    return result;
}

void H8300H::run()
{
    int result = 0;

    while (1) {
        if (interrupt_queue.hasInterrupt()) {
            // todo: 多重割り込みのブロック

            save_pc_and_ccr_to_stack();
            interrupt_t type = interrupt_queue.pop();
            pc = memory.get_vector(type);
        }

        result = step();
        if (result != 0) {
            break;
        }
    }
}