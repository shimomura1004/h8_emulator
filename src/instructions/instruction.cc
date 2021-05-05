#include "instruction.h"
#include <cstring>

Instruction::Instruction()
    : parser(nullptr)
    , runner(nullptr)
{}

int Instruction::run(H8Board* h8)
{
    return this->runner(h8, this);
}

void Instruction::stringify_name(char *buf)
{
    strcpy(buf, this->name);
}
