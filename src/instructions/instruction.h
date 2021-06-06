#ifndef _INSTRUCTION_INSTRUCTION_INCLUDED_
#define _INSTRUCTION_INSTRUCTION_INCLUDED_

#include "operator.h"
#include "board/h8board.h"

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

// TODO: h8instructions:: は h8::instruction:: に変える
namespace h8instructions {

// エミュレータの実行環境はリトルエンディアンが前提となっている
// 0x123456:24 は H8 のメモリ上では [0x12, 0x34, 0x56]
// エミュレータのメモリ(32ビット)上では [0x56, 0x34, 0x12, 0x00]
// 符号拡張が必要なので注意する
// [0xff, 0x12, 0x34] -> [0x34, 0x12, 0xff, 0xff]

// TODO: バイト配列にコピーしてキャストしなくても、単に加算していけばいいのでは？
/// H8 のアドレス start から T 型変数分のメモリを読み、T 型の値として返す
/// 4バイト変数に、3バイト分だけ読みたいときは size に 3 を指定する
template<class T>
T parse_immediate(H8Board *h8, uint8_t start, uint8_t size = sizeof(T))
{
    uint8_t imm[sizeof(T)] = {0};
    const uint8_t offset = sizeof(T) - size;

    for (int i=0; i < sizeof(T) - offset; i++) {
        imm[sizeof(T) - (i + offset) - 1] = h8->fetch_instruction_byte(start + i);
    }

    if (size != sizeof(T)) {
        // 符号拡張する
        bool msb = h8->fetch_instruction_byte(start) & 0x80;
        if (msb) {
            // 負の数の場合は 0xff で埋める
            for (int i=0; i < offset; i++) {
                imm[sizeof(T) - i - 1] = 0xff;
            }
        }
    }

    return *(T*)imm;
}

// value の正負により n,z を設定し、v をクリアする
template<class T>
void update_ccr_nzv(H8Board* h8, T value)
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
