#ifndef _INSTRUCTIONS_BCC_INCLUDED_
#define _INSTRUCTIONS_BCC_INCLUDED_

#include "board/h8board.h"

namespace h8instructions {
namespace bcc {

int bra_8(H8Board* h8);
int bra_16(H8Board* h8);
int bhi_8(H8Board* h8);
int bhi_16(H8Board* h8);
int bls_8(H8Board* h8);
int bls_16(H8Board* h8);
int bcc_8(H8Board* h8);
int bcs_8(H8Board* h8);
int bne_8(H8Board* h8);
int bne_16(H8Board* h8);
int beq_8(H8Board* h8);
int beq_16(H8Board* h8);
int bge_8(H8Board* h8);
int bge_16(H8Board* h8);
int blt_8(H8Board* h8);
int blt_16(H8Board* h8);
int bgt_8(H8Board* h8);
int bgt_16(H8Board* h8);
int ble_8(H8Board* h8);
int ble_16(H8Board* h8);

}
}

#endif
