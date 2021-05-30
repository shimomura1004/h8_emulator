#ifndef _INSTRUCTIONS_BCC_INCLUDED_
#define _INSTRUCTIONS_BCC_INCLUDED_

#include "board/h8board.h"
#include "instruction.h"

namespace h8instructions {
namespace bcc {

void bra_8_parse(H8Board* h8, Instruction& instruction);
int bra_8_run(H8Board* h8, Instruction& instruction);
void bra_16_parse(H8Board* h8, Instruction& instruction);
int bra_16_run(H8Board* h8, Instruction& instruction);

void bhi_8_parse(H8Board* h8, Instruction& instruction);
int bhi_8_run(H8Board* h8, Instruction& instruction);
void bhi_16_parse(H8Board* h8, Instruction& instruction);
int bhi_16_run(H8Board* h8, Instruction& instruction);

void bls_8_parse(H8Board* h8, Instruction& instruction);
int bls_8_run(H8Board* h8, Instruction& instruction);
void bls_16_parse(H8Board* h8, Instruction& instruction);
int bls_16_run(H8Board* h8, Instruction& instruction);

void bcc_8_parse(H8Board* h8, Instruction& instruction);
int bcc_8_run(H8Board* h8, Instruction& instruction);
void bcc_16_parse(H8Board* h8, Instruction& instruction);
int bcc_16_run(H8Board* h8, Instruction& instruction);

void bcs_8_parse(H8Board* h8, Instruction& instruction);
int bcs_8_run(H8Board* h8, Instruction& instruction);
void bcs_16_parse(H8Board* h8, Instruction& instruction);
int bcs_16_run(H8Board* h8, Instruction& instruction);

void bne_8_parse(H8Board* h8, Instruction& instruction);
int bne_8_run(H8Board* h8, Instruction& instruction);
void bne_16_parse(H8Board* h8, Instruction& instruction);
int bne_16_run(H8Board* h8, Instruction& instruction);

void beq_8_parse(H8Board* h8, Instruction& instruction);
int beq_8_run(H8Board* h8, Instruction& instruction);
void beq_16_parse(H8Board* h8, Instruction& instruction);
int beq_16_run(H8Board* h8, Instruction& instruction);

void bge_8_parse(H8Board* h8, Instruction& instruction);
int bge_8_run(H8Board* h8, Instruction& instruction);
void bge_16_parse(H8Board* h8, Instruction& instruction);
int bge_16_run(H8Board* h8, Instruction& instruction);

void blt_8_parse(H8Board* h8, Instruction& instruction);
int blt_8_run(H8Board* h8, Instruction& instruction);
void blt_16_parse(H8Board* h8, Instruction& instruction);
int blt_16_run(H8Board* h8, Instruction& instruction);

void bgt_8_parse(H8Board* h8, Instruction& instruction);
int bgt_8_run(H8Board* h8, Instruction& instruction);
void bgt_16_parse(H8Board* h8, Instruction& instruction);
int bgt_16_run(H8Board* h8, Instruction& instruction);

void ble_8_parse(H8Board* h8, Instruction& instruction);
int ble_8_run(H8Board* h8, Instruction& instruction);
void ble_16_parse(H8Board* h8, Instruction& instruction);
int ble_16_run(H8Board* h8, Instruction& instruction);

}
}

#endif
