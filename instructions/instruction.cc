#include "instruction.h"
#include <cstdio>
#include <cstring>

void Instruction::stringify_op(Operand& op, char *buf) {
    switch (op.mode) {
    case RegisterDirect8:
        sprintf(buf, "r%d%c", op.u8 % 8, (op.u8 < 8 ? 'h' : 'l'));
        break; 
    case RegisterDirect16:
        sprintf(buf, "%c%d", (op.u8 < 8 ? 'r' : 'e'), op.u8 % 8);
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
        sprintf(buf, "#0x%x", op.s8 & 0xff);
        break;
    case Immediate16:
        sprintf(buf, "#0x%x", op.s8 & 0xffff);
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
