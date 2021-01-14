#include "operation_map_5x.h"
#include "../instructions/jsr.h"
#include "../instructions/rts.h"
#include "../instructions/bcc.h"
#include "../instructions/rte.h"

instruction_handler_t lookup_58(H8300H *h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::bcc::bra_16;
    case 0x01: return nullptr; // BRN
    case 0x02: return nullptr; // BHI
    case 0x03: return nullptr; // BLS
    case 0x04: return nullptr; // BCC
    case 0x05: return nullptr; // BCS
    case 0x06: return h8instructions::bcc::bne_16;
    case 0x07: return h8instructions::bcc::beq_16;
    case 0x08: return nullptr; // BVC
    case 0x09: return nullptr; // BVS
    case 0x0a: return nullptr; // BPL
    case 0x0b: return nullptr; // BMI
    case 0x0c: return h8instructions::bcc::bge_16;
    case 0x0d: return h8instructions::bcc::blt_16;
    case 0x0e: return h8instructions::bcc::bgt_16;
    case 0x0f: return nullptr; // BLE
    default:   return nullptr;
    }
}

instruction_handler_t lookup_5x(H8300H *h8)
{
    unsigned char b0 = h8->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // MULXU
    case 0x01: return nullptr; // DIVXU
    case 0x02: return nullptr; // MULXU
    case 0x03: return nullptr; // DIVXU
    case 0x04: return h8instructions::rts::rts;
    case 0x05: return nullptr; // BSR
    case 0x06: return h8instructions::rte::rte;
    case 0x07: return nullptr; // TRAPA
    case 0x08: return lookup_58(h8);
    case 0x09:
    case 0x0a:
    case 0x0b: return nullptr; // JMP
    case 0x0c: return nullptr; // BSR
    case 0x0d: return h8instructions::jsr::register_indirect;
    case 0x0e: return h8instructions::jsr::absolute_address;
    case 0x0f: return nullptr; // JSR
    default:   return nullptr;
    }
}
