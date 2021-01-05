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
        fprintf(stderr, "Unknown instruction: 0x%02x at address 0x%08lx\n", first_byte, pc);
        result = -1;
        break;
    }

    return result;
}

void H8300H::load_elf(std::string filepath)
{
    memory.load_elf(filepath);
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
        // if (interrupt)
        //   proc_interrupt
        result = step();
        if (result != 0) {
            break;
        }
    }
}