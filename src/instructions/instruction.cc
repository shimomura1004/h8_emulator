#include "instruction.h"
#include <cstring>

Instruction::Instruction()
    : parser(nullptr)
    , runner(nullptr)
{}

int Instruction::run(H8Board* h8)
{
    return this->runner(h8, *this);
}

void Instruction::stringify_name(char *buf)
{
    strcpy(buf, this->name);
}

void Instruction::print()
{
    static char name[8];
    static char op1[32];
    static char op2[32];

    this->stringify_name(name);
    this->op1.stringify(op1);
    this->op2.stringify(op2);
    printf("%s %s,%s\n", name, op1, op2);
}