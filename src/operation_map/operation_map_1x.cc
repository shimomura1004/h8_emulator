#include "operation_map_1x.h"
#include "instructions/exts.h"
#include "instructions/shll.h"
#include "instructions/sub.h"
#include "instructions/subs.h"
#include "instructions/shlr.h"
#include "instructions/dec.h"
#include "instructions/extu.h"
#include "instructions/cmp.h"
#include "instructions/neg.h"
#include "instructions/xor.h"
#include "instructions/shar.h"
#include "instructions/rotxl.h"
#include "instructions/subx.h"
#include "instructions/rotl.h"
#include "instructions/not.h"

namespace operation_map {
instruction_parser_t lookup_10(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::shll::shll_b_parse;
    case 0x01: return h8instructions::shll::shll_w_parse;
    case 0x03: return h8instructions::shll::shll_l_parse;
    // case 0x08:
    // case 0x09: return nullptr; // SHAL
    // case 0x0b: return nullptr; // SHAL
    default:   return nullptr;
    }
}

instruction_parser_t lookup_11(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::shlr::shlr_b_parse;
    case 0x01: return h8instructions::shlr::shlr_w_parse;
    case 0x03: return h8instructions::shlr::shlr_l_parse;
    // case 0x08:
    case 0x09: return h8instructions::shar::shar_w_parse;
    case 0x0b: return h8instructions::shar::shar_l_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_12(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    // case 0x00: return nullptr; // rotxl.b
    // case 0x01: return nullptr; // rotxl.w
    case 0x03: return h8instructions::rotxl::rotxl_l_parse;
    case 0x08: return h8instructions::rotl::rotl_b_parse;
    // case 0x09: return nullptr; // ROTL
    // case 0x0b: return nullptr; // ROTL
    default:   return nullptr;
    }
}

instruction_parser_t lookup_13(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    // case 0x00:
    // case 0x01: return nullptr; // ROTXR
    // case 0x03: return nullptr; // ROTXR
    // case 0x08:
    // case 0x09: return nullptr; // ROTR
    // case 0x0b: return nullptr; // ROTR
    default:   return nullptr;
    }
}

instruction_parser_t lookup_17(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::h8not::not_b_parse;
    case 0x01: return h8instructions::h8not::not_w_parse;
    case 0x03: return h8instructions::h8not::not_l_parse;
    case 0x05: return h8instructions::extu::extu_w_parse;
    case 0x07: return h8instructions::extu::extu_l_parse;
    // case 0x08: return nullptr; // NEG.B
    case 0x09: return h8instructions::neg::neg_w_parse;
    // case 0x0b: return nullptr; // NEG
    case 0x0d: return h8instructions::exts::exts_w_parse;
    case 0x0f: return h8instructions::exts::exts_l_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_1A(H8Board* h8300h)
{
    uint8_t b1 = h8300h->fetch_instruction_byte(1);
    uint8_t bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::dec::dec_b_parse;
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return h8instructions::sub::register_direct_l_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_1B(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::subs::subs_parse;
    case 0x05: return h8instructions::dec::dec_w_parse;
    case 0x07: return h8instructions::dec::dec_l_parse;
    case 0x08:
    case 0x09: return h8instructions::subs::subs_parse;
    case 0x0d: return h8instructions::dec::dec_w_parse;
    case 0x0f: return h8instructions::dec::dec_l_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_1F(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    // case 0x00: return nullptr; // DAS
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return h8instructions::cmp::register_direct_l_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_1x(H8Board *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return lookup_10(h8300h);
    case 0x01: return lookup_11(h8300h);
    case 0x02: return lookup_12(h8300h);
    // case 0x03: return lookup_13(h8300h);
    // case 0x04: return nullptr; // OR
    case 0x05: return h8instructions::h8xor::register_direct_b_parse;
    // case 0x06: return nullptr; // AND
    case 0x07: return lookup_17(h8300h);
    case 0x08: return h8instructions::sub::register_direct_b_parse;
    case 0x09: return h8instructions::sub::register_direct_w_parse;
    case 0x0a: return lookup_1A(h8300h);
    case 0x0b: return lookup_1B(h8300h);
    case 0x0c: return h8instructions::cmp::register_direct_b_parse;
    case 0x0d: return h8instructions::cmp::register_direct_w_parse;
    case 0x0e: return h8instructions::subx::register_direct_parse;
    case 0x0f: return lookup_1F(h8300h);
    default:   return nullptr;
    }
}
}
