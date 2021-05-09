#ifndef _INSTRUCTION_INSTRUCTION_INCLUDED_
#define _INSTRUCTION_INSTRUCTION_INCLUDED_

#include "operator.h"
#include "board/h8board.h"

// todo: add と mov が表現できればたぶん大丈夫

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

#endif
