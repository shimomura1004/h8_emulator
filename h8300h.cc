#include "h8300h.h"
#include "operation_map/operation_map.h"
#include "sci/sci.h"

// todo: load コマンドで受信開始が異常に遅いことがある
// todo: 次の命令を判別だけする関数がほしい

uint8_t H8300H::fetch_instruction_byte(uint8_t offset)
{
    return mcu.read8(pc + offset);
}

int H8300H::execute_next_instruction()
{
    instruction_handler_t handler = OperationMap::lookup(this);

    if (handler == nullptr) {
        uint8_t first_byte = fetch_instruction_byte(0);
        fprintf(stderr, "Unknown instruction: [0x%02x, ...] at address 0x%06x\n", first_byte, pc);
        return -1;
    }
    
    // todo: デバッグ情報を取りやすくするため、パースと実行をわけたい
    // lookup のとき、関数ポインタだけでなく、引数をパースした結果まで含める
    // 戻り値は { pointer, set<パラメタ名->値> } な構造体とする
    int result = handler(this);
    if (result != 0) {
        uint8_t first_byte = fetch_instruction_byte(0);
        fprintf(stderr, "Instruction execution error(%d): [0x%02x, ...] at address 0x%06x\n", result, first_byte, pc);
    }

    return result;
}

// todo: スタック操作関係は別クラスに移動
void H8300H::push_to_stack_b(uint8_t value, uint8_t register_index)
{
    Register32& r = reg[register_index];
    r.set(r.get() - 1);
    mcu.write8(r.get(), value);
}

uint8_t H8300H::pop_from_stack_b(uint8_t register_index)
{
    Register32& r = reg[register_index];
    uint8_t value = mcu.read8(r.get());
    r.set(r.get() + 1);
    return value;
}

void H8300H::push_to_stack_w(uint16_t value, uint8_t register_index)
{
    Register32& r = reg[register_index];
    r.set(r.get() - 2);
    mcu.write16(r.get(), value);
}

uint16_t H8300H::pop_from_stack_w(uint8_t register_index)
{
    Register32& r = reg[register_index];
    uint16_t value = mcu.read16(r.get());
    r.set(r.get() + 2);
    return value;
}

void H8300H::push_to_stack_l(uint32_t value, uint8_t register_index)
{
    Register32& r = reg[register_index];
    r.set(r.get() - 4);
    mcu.write32(r.get(), value);
}

uint32_t H8300H::pop_from_stack_l(uint8_t register_index)
{
    Register32& r = reg[register_index];
    uint32_t value = mcu.read32(r.get());
    r.set(r.get() + 4);
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
    , reg16{ Register16(reg[0],  0), Register16(reg[1],  1), Register16(reg[2],  2), Register16(reg[3],  3),
             Register16(reg[4],  4), Register16(reg[5],  5), Register16(reg[6],  6), Register16(reg[7],  7),
             Register16(reg[0],  8), Register16(reg[1],  9), Register16(reg[2], 10), Register16(reg[3], 11),
             Register16(reg[4], 12), Register16(reg[5], 13), Register16(reg[6], 14), Register16(reg[7], 15) }
    ,  reg8{ Register8(reg[0],  0), Register8(reg[1],  1), Register8(reg[2],  2), Register8(reg[3],  3),
             Register8(reg[4],  4), Register8(reg[5],  5), Register8(reg[6],  6), Register8(reg[7],  7),
             Register8(reg[0],  8), Register8(reg[1],  9), Register8(reg[2], 10), Register8(reg[3], 11),
             Register8(reg[4], 12), Register8(reg[5], 13), Register8(reg[6], 14), Register8(reg[7], 15) }
    , pc(0)
    , mcu(interrupt_controller, mutex)
    , terminate(false)
    , is_sleep(false)
{
}

void H8300H::init()
{
}

uint32_t H8300H::load_elf(std::string filepath)
{
    return mcu.load_elf(filepath);
}

bool H8300H::handle_interrupt()
{
    // 割り込みがあれば処理
    interrupt_t type = interrupt_controller.getInterruptType();

    // todo: 内部割込みは CCR.I がセットされていてもブロックされないが、
    //       外部割り込みの場合は CCR.I がセットされている場合はブロックされる
    if (type != interrupt_t::NONE) {
        // 内部割込みの場合はすぐにクリアする
        // todo: 外部割り込みの場合は勝手にクリアしてはいけない
        interrupt_controller.clear(type);

        // CCR と PC を退避
        // H8 では、現在のスタックポインタの指す場所に退避される
        save_pc_and_ccr_to_stack();

        // H8 では、割り込みが発生すると勝手に CCR.I がセットされる
        ccr.set_i();

        // 割り込みベクタに設定されたアドレスにジャンプ
        pc = mcu.get_vector(type);

        return true;
    } else {
        return false;
    }
}

int H8300H::step()
{
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
        fprintf(stderr, "ER%d: 0x%08x", i, reg[i].get());
        fprintf(stderr, "    E%d: 0x%04x  R%d: 0x%04x", i, reg16[i + 8].get(), i, reg16[i].get());
        fprintf(stderr, "    RH%d: 0x%02x  RL%d: 0x%02x\n", i, reg8[i].get(), i, reg8[i + 8].get());
    }
    fprintf(stderr, "PC : 0x%08x\n", pc);
    fprintf(stderr, "CCR: I:%d, UI:%d, H:%d, U:%d, N:%d, Z:%d, V:%d, C:%d\n",
           ccr.i(), ccr.ui(), ccr.h(), ccr.u(), ccr.n(), ccr.z(), ccr.v(), ccr.c());
}
