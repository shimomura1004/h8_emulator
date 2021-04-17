#ifndef _INSTRUCTION_INSTRUCTION_INCLUDED_
#define _INSTRUCTION_INSTRUCTION_INCLUDED_

#include "h8300h.h"
#include <cstdint>

// todo: add と mov が表現できればたぶん大丈夫

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
    Immediate32,
    ProgramCounterRelative8,
    ProgramCounterRelative16,
    MemoryIndirect,
};

// todo: 別ファイルにする
struct Operand {
    addressing_mode_t mode;
    union {
        int8_t   s8;
        int16_t  s16;
        int32_t  s32;
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        struct {
            uint8_t disp16_register_index;
            int16_t disp16_displacement;
        };
        struct {
            uint8_t disp24_register_index;
            int32_t disp24_displacement;
        };
    };

    void set_register_direct8(uint8_t register_index);
    void set_register_direct16(uint8_t register_index);
    void set_register_direct32(uint8_t register_index);
    void set_register_indirect(uint8_t register_index);
    void set_register_indirect_with_displacement16(uint8_t register_index, int16_t displacement);
    void set_register_indirect_with_displacement24(uint8_t register_index, int32_t displacement);
    void set_register_indirect_with_post_incement(uint8_t register_index);
    void set_register_indirect_with_pre_decrement(uint8_t register_index);
    void set_absolute_address8(uint8_t address);
    void set_absolute_address16(uint16_t address);
    void set_absolute_address24(uint32_t address);
    void set_immediate8(int8_t immediate);
    void set_immediate16(int16_t immediate);
    void set_immediate32(int32_t immediate);
    void set_program_counter_relative8(int8_t offset);
    void set_program_counter_relative16(int16_t offset);
    void set_memory_indirect(uint8_t address);

    uint8_t  get_register_direct8();
    uint8_t  get_register_direct16();
    uint8_t  get_register_direct32();
    uint8_t  get_register_indirect();
    uint8_t  get_register_indirect_with_displacement16_register();
    int16_t  get_register_indirect_with_displacement16_displacement();
    uint8_t  get_register_indirect_with_displacement24_register();
    int32_t  get_register_indirect_with_displacement24_displacement();
    uint8_t  get_register_indirect_with_post_incement();
    uint8_t  get_register_indirect_with_pre_decrement();
    uint8_t  get_absolute_address8();
    uint16_t get_absolute_address16();
    uint32_t get_absolute_address24();
    int8_t   get_immediate8();
    int16_t  get_immediate16();
    int32_t  get_immediate32();
    int8_t   get_program_counter_relative8();
    int16_t  get_program_counter_relative16();
    uint8_t  get_memory_indirect();

};

class Instruction;
typedef void(*instruction_parser_t)(H8300H*, Instruction*);
typedef int(*instruction_runner_t)(H8300H*, Instruction*);

// parse すると run と help に値が入る
class Instruction {
protected:
    void stringify_op(Operand& op, char *buf);

public:
    instruction_parser_t parser;
    instruction_runner_t runner;

    const char *name;
    Operand op1;
    Operand op2;

public:
    int run(H8300H* h8) { return this->runner(h8, this); }

    void stringify_name(char *buf);
    void stringify_op1(char *buf);
    void stringify_op2(char *buf);

};

#endif
