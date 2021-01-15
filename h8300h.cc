#include "h8300h.h"
#include "operation_map/operation_map.h"
#include "sci/sci.h"

// todo: load コマンドが異常に遅い

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

void H8300H::restore_pc_and_ccr_from_stack()
{
    uint32_t ccr_pc = pop_from_stack_l();
    ccr.set(ccr_pc >> 24);
    pc = ccr_pc & 0x00ffffff;
}

H8300H::H8300H()
    : sp(reg[7])
    , pc(0)
    , terminate(false)
    , is_sleep(false)
{}

H8300H::~H8300H()
{
    if (sci1) {
        delete sci1;
    }
}

void H8300H::init()
{
    sci1 = new Sci(1, memory, interrupt_controller, terminate, mutex);
}

uint32_t H8300H::load_elf(std::string filepath)
{
    return memory.load_elf(filepath);
}

int H8300H::step()
{
    // 割り込みがあれば処理
    interrupt_t type = interrupt_controller.getInterruptType();
    if (type != interrupt_t::NONE && !ccr.i()) {
        // CCR と PC を退避
        // H8 では、現在のスタックポインタの指す場所に退避される
        save_pc_and_ccr_to_stack();

        // H8 では、割り込みが発生すると勝手に CCR.I がセットされる
        ccr.set_i();

        // 割り込みベクタに設定されたアドレスにジャンプ
        pc = memory.get_vector(type);
    }

    // PC が指す命令を実行
    int result = execute_next_instruction();

    if (result != 0) {
        fprintf(stderr, "Abort.\n");
    }

    if (is_sleep) {
        // スリープ状態の場合は wait する
        // 復帰するときは別スレッドから notify する必要がある
        interrupt_controller.wait_for_interruption();
        is_sleep = false;
    }

    return result;
}

void H8300H::print_registers()
{
    for (int i = 0; i < 8; i++) {
        printf("ER%d: 0x%08x", i, reg[i].get_er());
        printf("    E%d: 0x%04x  R%d: 0x%04x", i, reg[i].get_e(), i, reg[i].get_r());
        printf("    RH%d: 0x%02x  RL%d: 0x%02x\n", i, reg[i].get_rh(), i, reg[i].get_rl());
    }
    printf("PC : 0x%08x\n", pc);
    printf("CCR: I:%d, UI:%d, H:%d, U:%d, N:%d, Z:%d, V:%d, C:%d\n",
           ccr.i(), ccr.ui(), ccr.h(), ccr.u(), ccr.n(), ccr.z(), ccr.v(), ccr.c());
}
