#ifndef _INSTRUCTION_INSTRUCTION_INCLUDED_
#define _INSTRUCTION_INSTRUCTION_INCLUDED_

#include "instruction_info.h"

// todo: add と mov が表現できればたぶん大丈夫

// todo: アドレッシングモードは不要か？
// Rn                       レジスタ直接
// @ERn                     レジスタ間接
// @(d:16,ERn)/@(d:24,ERn)  ディスプレースメント（16/24 ビット）付レジスタ間接
// @ERn+/@-ERn              ポストインクリメントレジスタ間接／プリデクリメントレジスタ間接
// @aa:8/@aa:16/@aa:24      絶対アドレス（8/16/24 ビット）
// #xx:8/#xx:16/#xx:32      イミディエイト（8/16/32 ビット）
// @(d:8,PC)/@(d:16, PC)    プログラムカウンタ相対（8/16 ビット）
// @@aa:8                   メモリ間接
enum addressing_mode_t {
    RegisterDirect8,
    RegisterDirect16,
    RegisterDirect32,
    RegisterIndirect,
    RegisterIndirectWithDisplacement16,
    RegisterIndirectWithDisplacement24,
    RegisterIndirectWithPostIncement,
    RegisterIndirectWithPreDecrement,
    AbsoluteAddress8,
    AbsoluteAddress16,
    AbsoluteAddress24,
    Immediate8,
    Immediate16,
    Immediate24,
    ProgramCounterRelative8,
    ProgramCounterRelative16,
    MemoryIndirect,
};

struct Operand {
    addressing_mode_t mode;
    union {
        int8_t   s8;
        int16_t  s16;
        int32_t  s32;
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
    };
};

class Instruction;
typedef void(*instruction_parser_t)(H8300H*, Instruction*);
typedef int(*instruction_runner_t)(H8300H*, Instruction*);
// typedef InstructionInfo*(*instruction_helper_t)(H8300H*, Instruction&);

// instruction をクラスにしてメソッドを追加
// operand に enum を追加して print するようにする

// parse すると run と help に値が入る
class Instruction {
protected:
    void stringify_op(Operand& op, char *buf) {
        switch (op.mode) {
        case RegisterDirect8:
            sprintf(buf, "r%d%c", op.u8 % 8, (op.u8 < 8 ? 'h' : 'l'));
            break; 
        case RegisterDirect16:
            break;
        case RegisterDirect32:
            break;
        case RegisterIndirect:
            break;
        case RegisterIndirectWithDisplacement16:
            break;
        case RegisterIndirectWithDisplacement24:
            break;
        case RegisterIndirectWithPostIncement:
            break;
        case RegisterIndirectWithPreDecrement:
            break;
        case AbsoluteAddress8:
            break;
        case AbsoluteAddress16:
            break;
        case AbsoluteAddress24:
            break;
        case Immediate8:
            sprintf(buf, "#0x%02x", op.s8 & 0xff);
            break;
        case Immediate16:
            break;
        case Immediate24:
            break;
        case ProgramCounterRelative8:
            break;
        case ProgramCounterRelative16:
            break;
        case MemoryIndirect:
            break;
        default:
            fprintf(stderr, "Error: Unknown addressing mode\n");
            return;
        }
    }

public:
    instruction_parser_t parser;
    instruction_runner_t runner;
    // instruction_helper_t help;
    const char *name;
    Operand op1;
    Operand op2;

public:
    int run(H8300H* h8) { return this->runner(h8, this); }

    void stringify_name(char *buf) { strcpy(buf, this->name); }
    void stringify_op1(char *buf) { stringify_op(this->op1, buf); }
    void stringify_op2(char *buf) { stringify_op(this->op2, buf); }

};

#endif
