#ifndef _INSTRUCTION_OPERATOR_INCLUDED_
#define _INSTRUCTION_OPERATOR_INCLUDED_

#include <cstdint>

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

class Operand {
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

public:
    void stringify(char *buf);

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

#endif
