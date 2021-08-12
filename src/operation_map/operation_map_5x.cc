#include "operation_map_5x.h"
#include "instructions/jsr.h"
#include "instructions/rts.h"
#include "instructions/bcc.h"
#include "instructions/rte.h"
#include "instructions/trapa.h"
#include "instructions/jmp.h"

namespace operation_map {

instruction_parser_t lookup_58(H8Board *h8)
{
    unsigned char b1 = h8->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::bcc::bra_16_parse;
    case 0x01: return nullptr; // BRN
    case 0x02: return h8instructions::bcc::bhi_16_parse;
    case 0x03: return h8instructions::bcc::bls_16_parse;
    case 0x04: return nullptr; // BCC
    case 0x05: return nullptr; // BCS
    case 0x06: return h8instructions::bcc::bne_16_parse;
    case 0x07: return h8instructions::bcc::beq_16_parse;
    case 0x08: return nullptr; // BVC
    case 0x09: return nullptr; // BVS
    case 0x0a: return nullptr; // BPL
    case 0x0b: return nullptr; // BMI
    case 0x0c: return h8instructions::bcc::bge_16_parse;
    case 0x0d: return h8instructions::bcc::blt_16_parse;
    case 0x0e: return h8instructions::bcc::bgt_16_parse;
    case 0x0f: return h8instructions::bcc::ble_16_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_5x(H8Board *h8)
{
    unsigned char b0 = h8->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // MULXU
    case 0x01: return nullptr; // DIVXU
    case 0x02: return nullptr; // MULXU
    case 0x03: return nullptr; // DIVXU
    case 0x04: return h8instructions::rts::rts_parse;
    case 0x05: return nullptr; // BSR
    case 0x06: return h8instructions::rte::rte_parse;
    case 0x07: return h8instructions::trapa::trapa_parse;
    case 0x08: return lookup_58(h8);
    case 0x09: return h8instructions::jmp::register_indirect_parse;
    case 0x0a: return nullptr; // JMP
    case 0x0b: return nullptr; // JMP
    case 0x0c: return nullptr; // BSR
    case 0x0d: return h8instructions::jsr::register_indirect_parse;
    case 0x0e: return h8instructions::jsr::absolute_address_parse;
    case 0x0f: return nullptr; // JSR
    default:   return nullptr;
    }
}

}