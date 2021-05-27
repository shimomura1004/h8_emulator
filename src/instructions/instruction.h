#ifndef _INSTRUCTION_INSTRUCTION_INCLUDED_
#define _INSTRUCTION_INSTRUCTION_INCLUDED_

#include "operator.h"
#include "board/h8board.h"

// TODO: add と mov が表現できればたぶん大丈夫

class Instruction;
typedef void(*instruction_parser_t)(H8Board*, Instruction&);
typedef int(*instruction_runner_t)(H8Board*, Instruction&);

// 呼び出し元からはパースするまで命令の種類がわからないが、
// 継承を使って表現すると毎回メモリ領域をヒープに確保されてしまい効率が悪いので、
// 全ての命令・アドレッシングモードに対応できる共用体を呼び出し元で確保してそこにパース結果を入れる
// パースすると name/op1/op2 に値が入る他、run にも対応する命令の関数ポインタが入る
class Instruction {
public:
    instruction_parser_t parser;
    instruction_runner_t runner;

    const char *name;
    Operand op1;
    Operand op2;

public:
    Instruction();
    int run(H8Board* h8);
    void stringify_name(char *buf);

    void print();

};

namespace h8instructions {

template<class T>
T parse_immediate(H8Board *h8, uint8_t offset)
{
    uint8_t imm[sizeof(T)];

    for (int i=0; i < sizeof(T); i++) {
        imm[sizeof(T) - i - 1] = h8->fetch_instruction_byte(offset + i);
    }

    return *(T*)imm;
}

template<class T>
void update_ccr(H8Board* h8, T value)
{
    if (value < 0) {
        h8->cpu.ccr().set_n();
    } else {
        h8->cpu.ccr().clear_n();
    }

    if (value == 0) {
        h8->cpu.ccr().set_z();
    } else {
        h8->cpu.ccr().clear_z();
    }

    h8->cpu.ccr().clear_v();
}

}

#endif
