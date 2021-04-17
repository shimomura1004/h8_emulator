#include "instruction.h"
#include <cstdio>
#include <cstring>

void Operand::set_register_direct8(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterDirect8;
    this->u8 = register_index;
}

void Operand::set_register_direct16(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterDirect16;
    this->u8 = register_index;
}

void Operand::set_register_direct32(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterDirect32;
    this->u8 = register_index;
}

void Operand::set_register_indirect(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterIndirect;
    this->u8 = register_index;
}

void Operand::set_register_indirect_with_displacement16(uint8_t register_index, int16_t displacement)
{
    this->mode = addressing_mode_t::RegisterIndirectWithDisplacement16;
    this->disp16_register_index = register_index;
    this->disp16_displacement = displacement;
}

void Operand::set_register_indirect_with_displacement24(uint8_t register_index, int32_t displacement)
{
    this->mode = addressing_mode_t::RegisterIndirectWithDisplacement24;
    this->disp24_register_index = register_index;
    this->disp24_displacement = displacement;
}

void Operand::set_register_indirect_with_post_incement(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterIndirectWithPostIncement;
    this->u8 = register_index;
}

void Operand::set_register_indirect_with_pre_decrement(uint8_t register_index)
{
    this->mode = addressing_mode_t::RegisterIndirectWithPreDecrement;
    this->u8 = register_index;
}

void Operand::set_absolute_address8(uint8_t address)
{
    this->mode = addressing_mode_t::AbsoluteAddress8;
    this->u8 = address;
}

void Operand::set_absolute_address16(uint16_t address)
{
    this->mode = addressing_mode_t::AbsoluteAddress16;
    this->u16 = address;
}

void Operand::set_absolute_address24(uint32_t address)
{
    this->mode = addressing_mode_t::AbsoluteAddress24;
    this->u32 = address;
}

void Operand::set_immediate8(int8_t immediate)
{
    this->mode = addressing_mode_t::Immediate8;
    this->s8 = immediate;
}

void Operand::set_immediate16(int16_t immediate)
{
    this->mode = addressing_mode_t::Immediate16;
    this->s16 = immediate;
}

void Operand::set_immediate32(int32_t immediate)
{
    this->mode = addressing_mode_t::Immediate32;
    this->s32 = immediate;
}

void Operand::set_program_counter_relative8(int8_t offset)
{
    this->mode = addressing_mode_t::ProgramCounterRelative8;
    this->s8 = offset;
}

void Operand::set_program_counter_relative16(int16_t offset)
{
    this->mode = addressing_mode_t::ProgramCounterRelative16;
    this->s16 = offset;
}

void Operand::set_memory_indirect(uint8_t address)
{
    this->mode = addressing_mode_t::MemoryIndirect;
    this->u8 = address;
}

uint8_t Operand::get_register_direct8()
{
    if (this->mode != addressing_mode_t::RegisterDirect8) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_register_direct16()
{
    if (this->mode != addressing_mode_t::RegisterDirect16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_register_direct32()
{
    if (this->mode != addressing_mode_t::RegisterDirect32) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_register_indirect()
{
    if (this->mode != addressing_mode_t::RegisterIndirect) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_register_indirect_with_displacement16_register()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithDisplacement16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->disp16_register_index;
}

int16_t Operand::get_register_indirect_with_displacement16_displacement()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithDisplacement16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->disp16_displacement;
}

uint8_t Operand::get_register_indirect_with_displacement24_register()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithDisplacement24) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->disp24_register_index;
}

int32_t Operand::get_register_indirect_with_displacement24_displacement()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithDisplacement24) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->disp24_displacement;
}

uint8_t Operand::get_register_indirect_with_post_incement()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithPostIncement) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_register_indirect_with_pre_decrement()
{
    if (this->mode != addressing_mode_t::RegisterIndirectWithPreDecrement) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint8_t Operand::get_absolute_address8()
{
    if (this->mode != addressing_mode_t::AbsoluteAddress8) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

uint16_t Operand::get_absolute_address16()
{
    if (this->mode != addressing_mode_t::AbsoluteAddress16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u16;
}

uint32_t Operand::get_absolute_address24()
{
    if (this->mode != addressing_mode_t::AbsoluteAddress24) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u32;
}

int8_t Operand::get_immediate8()
{
    if (this->mode != addressing_mode_t::Immediate8) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->s8;
}

int16_t Operand::get_immediate16()
{
    if (this->mode != addressing_mode_t::Immediate16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->s16;
}

int32_t Operand::get_immediate32()
{
    if (this->mode != addressing_mode_t::Immediate32) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->s32;
}

int8_t Operand::get_program_counter_relative8()
{
    if (this->mode != addressing_mode_t::ProgramCounterRelative8) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->s8;
}

int16_t Operand::get_program_counter_relative16()
{
    if (this->mode != addressing_mode_t::ProgramCounterRelative16) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->s16;
}

uint8_t Operand::get_memory_indirect()
{
    if (this->mode != addressing_mode_t::MemoryIndirect) {
        fprintf(stderr, "Error: Operand type mismatch(%s)\n", __func__);
    }
    return this->u8;
}

void Instruction::stringify_op(Operand& op, char *buf) {
    switch (op.mode) {
    case RegisterDirect8:
        sprintf(buf, "r%d%c", op.u8 % 8, (op.u8 < 8 ? 'h' : 'l'));
        break; 
    case RegisterDirect16:
        sprintf(buf, "%c%d", (op.u8 < 8 ? 'r' : 'e'), op.u8 % 8);
        break;
    case RegisterDirect32:
        sprintf(buf, "er%d", op.u8);
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
        sprintf(buf, "#0x%x", op.s8 & 0xff);
        break;
    case Immediate16:
        sprintf(buf, "#0x%x", op.s16 & 0xffff);
        break;
    case Immediate32:
        sprintf(buf, "#0x%x", op.s32 & 0xffffffff);
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

void Instruction::stringify_name(char *buf)
{
    strcpy(buf, this->name);
}

void Instruction::stringify_op1(char *buf)
{
    stringify_op(this->op1, buf);
}

void Instruction::stringify_op2(char *buf)
{
    stringify_op(this->op2, buf);
}
