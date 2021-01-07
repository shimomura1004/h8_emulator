#include "h8300h.h"
#include "operation_map/operation_map.h"
#include "sci.h"

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
void H8300H::push_to_stack_b(uint8_t value, unsigned int register_index)
{
    Register32& r = reg[register_index];
    r.set_er(r.get_er() - 1);
    memory.write_uint8(r.get_er(), value);
}

uint8_t H8300H::pop_from_stack_b(unsigned int register_index)
{
    Register32& r = reg[register_index];
    uint8_t value = memory.read_uint8(r.get_er());
    r.set_er(r.get_er() + 1);
    return value;
}

void H8300H::push_to_stack_w(uint16_t value, unsigned int register_index)
{
    Register32& r = reg[register_index];
    r.set_er(r.get_er() - 2);
    memory.write_uint16(r.get_er(), value);
}

uint16_t H8300H::pop_from_stack_w(unsigned int register_index)
{
    Register32& r = reg[register_index];
    uint16_t value = memory.read_uint16(r.get_er());
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
    uint32_t value = memory.read_uint32(r.get_er());
    r.set_er(r.get_er() + 4);
    return value;
}

void H8300H::save_pc_and_ccr_to_stack()
{
    uint32_t ccr_pc = pc | (ccr.raw() << 24);
    push_to_stack_l(ccr_pc);
}

H8300H::~H8300H()
{
    for (int i = 0; i < 3; i++) {
        if (sci[i]) {
            if (sci[i]->joinable()) {
                sci[i]->join();
            }
            delete sci[i];
        }
    }
}

void H8300H::init()
{
    for (uint8_t i = 0; i < 3; i++) {
        if (i==1)
        sci[i] = new std::thread(&sci::start, i, std::ref(memory), std::ref(terminate));
    }
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

        printf("PC : 0x%08x\n", pc);
        // print_registers();

        result = step();
        if (result != 0) {
            fprintf(stderr, "Core dumped.\n");
            memory.dump("core");
            break;
        }
    }

    terminate = true;
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
