#ifndef _INSTRUCTIONS_BCC_INCLUDED_
#define _INSTRUCTIONS_BCC_INCLUDED_

#include "../h8300h.h"

namespace h8instructions {
namespace bcc {

int bra_8(H8300H* h8);
int bra_16(H8300H* h8);
int bls_8(H8300H* h8);
int bcc_8(H8300H* h8);
int bne_8(H8300H* h8);
int bne_16(H8300H* h8);
int beq_8(H8300H* h8);
int beq_16(H8300H* h8);
int bge_8(H8300H* h8);
int blt_16(H8300H* h8);
int bgt_8(H8300H* h8);
int ble_8(H8300H* h8);

}
}

#endif
