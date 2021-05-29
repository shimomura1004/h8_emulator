#include "operation_map_4x.h"
#include "instructions/bcc.h"

namespace operation_map {

instruction_handler_t lookup_4x(H8Board *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x0c: return h8instructions::bcc::bge_8;
    case 0x0d: return h8instructions::bcc::blt_8;
    case 0x0e: return h8instructions::bcc::bgt_8;
    case 0x0f: return h8instructions::bcc::ble_8;
    default:   return nullptr;
    }
}

}


namespace operation_map2 {

instruction_parser_t lookup_4x(H8Board *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return h8instructions::bcc::bra_8_parse;
    case 0x01: return nullptr; // BRN
    case 0x02: return h8instructions::bcc::bhi_8_parse;
    case 0x03: return h8instructions::bcc::bls_8_parse;
    case 0x04: return h8instructions::bcc::bcc_8_parse;
    case 0x05: return h8instructions::bcc::bcs_8_parse;
    case 0x06: return h8instructions::bcc::bne_8_parse;
    case 0x07: return h8instructions::bcc::beq_8_parse;
    case 0x08: return nullptr; // BVC
    case 0x09: return nullptr; // BVS
    case 0x0a: return nullptr; // BPL
    case 0x0b: return nullptr; // BMI
    // case 0x0c: return h8instructions::bcc::bge_8_parse;
    // case 0x0d: return h8instructions::bcc::blt_8_parse;
    // case 0x0e: return h8instructions::bcc::bgt_8_parse;
    // case 0x0f: return h8instructions::bcc::ble_8_parse;
    default:   return nullptr;
    }
}

}
