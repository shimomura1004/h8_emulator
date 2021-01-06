#include "h8300h.h"
#include "operation_map/operation_map.h"

unsigned char H8300H::fetch_instruction_byte(unsigned int offset)
{
    return memory[pc + offset];
}

int H8300H::execute_next_instruction()
{
    instruction_handler_t handler = OperationMap::lookup(this);

    if (handler == nullptr) {
        unsigned char first_byte = fetch_instruction_byte(0);
        fprintf(stderr, "Unknown instruction: [0x%02x, ...] at address 0x%08x\n", first_byte, pc);
        return -1;
    }
    
    int result = handler(this);
    if (result != 0) {
        unsigned char first_byte = fetch_instruction_byte(0);
        fprintf(stderr, "Instruction execution error(%d): [0x%02x, ...] at address 0x%08x\n", result, first_byte, pc);
    }

    return result;
}

// todo: スタック操作関係は別クラスに移動
void H8300H::push_to_stack_w(uint16_t value, unsigned int register_index)
{
    Register32& r = reg[register_index];
    r.set_er(r.get_er() - 2);
    unsigned char mem = memory[r.get_er()];
    *(uint16_t*)&mem = value;
}

uint16_t H8300H::pop_from_stack_w(unsigned int register_index)
{
    Register32& r = reg[register_index];
    unsigned char mem = memory[r.get_er()];
    unsigned short value = *(uint16_t*)&mem;
    r.set_er(r.get_er() + 2);
    return value;
}

void H8300H::push_to_stack_l(uint32_t value, unsigned int register_index)
{
    Register32& r = reg[register_index];
    r.set_er(r.get_er() - 4);
    memory.write_uint32(r.get_er(), value);
}

uint32_t H8300H::pop_from_stack_l(unsigned int register_index)
{
    Register32& r = reg[register_index];
    unsigned char mem = memory[r.get_er()];
    uint32_t value = *(uint32_t*)&mem;
    r.set_er(r.get_er() + 4);
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
            fprintf(stderr, "Core dumped.\n");
            memory.dump("core");
            break;
        }
    }
}

void H8300H::print_registers()
{
    for (int i = 0; i < 8; i++) {
        const unsigned char* raw = reg[i].raw();
        printf("ER%d: 0x%02x%02x%02x%02x\n", i, raw[0], raw[1], raw[2], raw[3]);
    }
    printf("PC : 0x%08x\n", pc);
    printf("CCR: I:%d, UI:%d, H:%d, U:%d, N:%d, Z:%d, V:%d, C:%d\n",
           ccr.i(), ccr.ui(), ccr.h(), ccr.u(), ccr.n(), ccr.z(), ccr.v(), ccr.c());
}
