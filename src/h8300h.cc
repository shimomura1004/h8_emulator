#include "h8300h.h"
#include "operation_map/operation_map.h"

// todo: H8300H_DRAM/H8300H_SCI/H8300H_Timer8/RTL8019AS を外部から注入すれば不要
#include "dram/h8300h_dram.h"
#include "sci/h8300h_sci.h"
#include "timer/h8300h_timer8.h"
#include "net/rtl8019as.h"
#include "interrupt/general_interrupt_controller.h"

// todo: デバッグ用
#include <set>

uint8_t H8300H::fetch_instruction_byte(uint8_t offset)
{
    return mcu.read8(this->cpu.pc() + offset);
}

int H8300H::execute_next_instruction()
{
    instruction_parser_t parser = operation_map2::lookup(this);

    if (parser) {
        Instruction instruction;
        parser(this, &instruction);

        {
            // 初回のみ命令を print し動作確認
            static std::set<instruction_parser_t> tmp;
            if (tmp.find(parser) == tmp.end()) {
                tmp.insert(parser);
                
                char name[8];
                char op1[32];
                char op2[32];
                instruction.stringify_name(name);
                instruction.op1.stringify(op1);
                instruction.op2.stringify(op2);
                printf("[0x%06x] %s %s,%s\n", this->cpu.pc(), name, op1, op2);
            }
        }

        int result = instruction.run(this);

        if (result != 0) {
            uint8_t first_byte = fetch_instruction_byte(0);
            fprintf(stderr, "Instruction execution error(%d): [0x%02x, ...] at address 0x%06x\n", result, first_byte, this->cpu.pc());
        }

        return result;
    } else {
        {
            // todo: 移行のためのフォールバック処理
            instruction_handler_t handler = operation_map::lookup(this);

            if (handler == nullptr) {
                uint8_t first_byte = fetch_instruction_byte(0);
                fprintf(stderr, "Unknown instruction: [0x%02x, ...] at address 0x%06x\n", first_byte, this->cpu.pc());
                return -1;
            }

            int result = handler(this);
            if (result != 0) {
                uint8_t first_byte = fetch_instruction_byte(0);
                fprintf(stderr, "Instruction execution error(%d): [0x%02x, ...] at address 0x%06x\n", result, first_byte, this->cpu.pc());
            }

            return result;
        }

        // uint8_t first_byte = fetch_instruction_byte(0);
        // fprintf(stderr, "Unknown instruction: [0x%02x, ...] at address 0x%06x\n", first_byte, this->cpu.pc());
        // return -1;
    }
}

// todo: スタック操作関係は別クラスに移動
void H8300H::push_to_stack_b(uint8_t value, uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    r.set(r.get() - 1);
    mcu.write8(r.get(), value);
}

uint8_t H8300H::pop_from_stack_b(uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    uint8_t value = mcu.read8(r.get());
    r.set(r.get() + 1);
    return value;
}

void H8300H::push_to_stack_w(uint16_t value, uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    r.set(r.get() - 2);
    mcu.write16(r.get(), value);
}

uint16_t H8300H::pop_from_stack_w(uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    uint16_t value = mcu.read16(r.get());
    r.set(r.get() + 2);
    return value;
}

void H8300H::push_to_stack_l(uint32_t value, uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    r.set(r.get() - 4);
    mcu.write32(r.get(), value);
}

uint32_t H8300H::pop_from_stack_l(uint8_t register_index)
{
    Register32& r = this->cpu.reg32(register_index);
    uint32_t value = mcu.read32(r.get());
    r.set(r.get() + 4);
    return value;
}

void H8300H::save_pc_and_ccr_to_stack()
{
    uint32_t ccr_pc = this->cpu.pc() | (this->cpu.ccr().raw() << 24);
    push_to_stack_l(ccr_pc);
}

void H8300H::restore_pc_and_ccr_from_stack()
{
    uint32_t ccr_pc = pop_from_stack_l();
    this->cpu.ccr().set(ccr_pc >> 24);
    this->cpu.pc() = ccr_pc & 0x00ffffff;
}

H8300H::H8300H(ICPU& cpu, bool use_stdio)
    : cpu(cpu)
    // todo: 外部から注入する
    , dram(new H8300H_DRAM())
    , sci{ new H8300H_SCI(0, interrupt_cv), new H8300H_SCI(1, interrupt_cv, use_stdio), new H8300H_SCI(2, interrupt_cv) }
    , timer8(new H8300H_Timer8(interrupt_cv))
    , ioport(new IOPort())
    , nic(new RTL8019AS(interrupt_cv))
    , mcu(dram, sci, timer8, ioport, nic)
    , interrupt_controller(new GeneralInterruptController(this->sci, this->timer8, this->nic))
    , terminate(false)
    , is_sleep(false)
{
}

H8300H::~H8300H()
{
    delete(this->dram);
    for (int i = 0; i < 3; i++) {
        delete this->sci[i];
    }
    delete this->timer8;
    delete this->ioport;
    delete this->nic;
    delete this->interrupt_controller;
}

void H8300H::init()
{
    for (int i = 0; i < 3; i++) {
        this->sci[i]->run();
    }

    this->nic->run();
}

uint32_t H8300H::load_elf(std::string filepath)
{
    return mcu.load_elf(filepath);
}

bool H8300H::handle_interrupt()
{
    interrupt_t type = interrupt_t::NONE;

    // 割込み可能な状態の場合、割り込みがあれば処理
    if (!this->cpu.ccr().i()) {
        type = interrupt_controller->getInterruptType();
    }

    // 割込み可能かどうかに関係なく、優先度の高い割込みがない場合はトラップされたかを確認
    if (type == interrupt_t::NONE) {
        type = interrupt_controller->getTrap();
    }

    if (type != interrupt_t::NONE) {
        // 割込み要求フラグをクリア
        interrupt_controller->clear(type);

        // todo: CPU 内に隠蔽できないか
        // CCR と PC を退避
        // H8 では、現在のスタックポインタの指す場所に退避される
        save_pc_and_ccr_to_stack();

        // H8 では、割り込みが発生すると勝手に CCR.I がセットされる
        this->cpu.ccr().set_i();

        // 割り込みベクタに設定されたアドレスにジャンプ
        this->cpu.pc() = mcu.get_vector(type);

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
        // 割込みコントローラから起こすようになっている

        // todo: CCR のロックを取ったほうがいいのでは？
        std::mutex tmp;
        std::unique_lock<std::mutex> lock(tmp);
        interrupt_cv.wait(lock, [this]{
            // スリープ中にトラップ命令がくることはないため、トラップは考慮不要
            // スリープ中に CCR.I が更新されることはないので、CCR.I が解除されたときに notify する必要はない
            interrupt_t type = this->interrupt_controller->getInterruptType();

            if (type == interrupt_t::NONE) {
                // 割込みがなければ待つ
                return false;
            } else if (type == interrupt_t::NMI) {
                // NMI は常に処理
                return true;
            } else if (!this->cpu.ccr().i()) {
                // それ以外の割込みの場合、割込み禁止状態でなければ処理
                return true;
            } else {
                // 割込み禁止状態の場合は待つ
                return false;
            }
        });
        is_sleep = false;
    }

    return result;
}

void H8300H::print_registers()
{
    for (int i = 0; i < 8; i++) {
        fprintf(stderr, "ER%d: 0x%08x", i, this->cpu.reg32(i).get());
        fprintf(stderr, "    E%d: 0x%04x  R%d: 0x%04x",
                i,
                this->cpu.reg16(i + 8).get(),
                i,
                this->cpu.reg16(i).get());
        fprintf(stderr, "    RH%d: 0x%02x  RL%d: 0x%02x\n",
                i,
                this->cpu.reg8(i).get(),
                i,
                this->cpu.reg8(i + 8).get());
    }
    fprintf(stderr, "PC : 0x%08x\n", this->cpu.pc());
    fprintf(stderr, "CCR: I:%d, UI:%d, H:%d, U:%d, N:%d, Z:%d, V:%d, C:%d\n",
           this->cpu.ccr().i(),
           this->cpu.ccr().ui(),
           this->cpu.ccr().h(),
           this->cpu.ccr().u(),
           this->cpu.ccr().n(),
           this->cpu.ccr().z(),
           this->cpu.ccr().v(),
           this->cpu.ccr().c());
}
