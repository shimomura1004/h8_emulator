#include "operation_map_7x.h"
#include "operation_map_7Cx.h"
#include "operation_map_7Dx.h"
#include "operation_map_7Ex.h"
#include "operation_map_7Fx.h"

#include "instructions/mov_register_indirect_displacement24.h"
#include "instructions/mov_immediate.h"
#include "instructions/and.h"
#include "instructions/add.h"
#include "instructions/sub.h"
#include "instructions/cmp.h"
#include "instructions/bclr.h"
#include "instructions/btst.h"
#include "instructions/bset.h"
#include "instructions/or.h"
#include "instructions/bld.h"

namespace operation_map {

instruction_handler_t lookup_7x(H8Board *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x03: return h8instructions::btst::btst_register_direct_imm;
    case 0x07: return lookup_77(h8300h);
    case 0x0a: return lookup_7A(h8300h);
    case 0x0c: return lookup_7Cr0xx(h8300h);
    case 0x0d: return lookup_7Dr0xx(h8300h);
    case 0x0e: return lookup_7Eaaxx(h8300h);
    case 0x0f: return lookup_7Faaxx(h8300h);
    default:   return nullptr;
    }
}


instruction_handler_t lookup_77(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? h8instructions::bld::bld_register_direct
                  : nullptr; // BILD
}

instruction_handler_t lookup_7A(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x03: return h8instructions::sub::sub_immediate_l;
    default:   return nullptr;
    }
}

}



namespace operation_map2 {

instruction_parser_t lookup_7x(H8Board *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return h8instructions::bset::register_direct_immediate_parse;
    // case 0x01: return nullptr; // BNOT
    case 0x02: return h8instructions::bclr::register_direct_immediate_parse;
    // case 0x03: return h8instructions::btst::btst_register_direct_imm;
    case 0x04: return lookup_74(h8300h);
    case 0x05: return lookup_75(h8300h);
    case 0x06: return lookup_76(h8300h);
    case 0x07: return lookup_77(h8300h);
    case 0x08: return h8instructions::mov::register_indirect_with_displacement24_b_parser;
    case 0x09: return lookup_79(h8300h);
    case 0x0a: return lookup_7A(h8300h);
    // case 0x0b: return nullptr; // EEPMOV
    // case 0x0c: return lookup_7Cr0xx(h8300h);
    // case 0x0d: return lookup_7Dr0xx(h8300h);
    // case 0x0e: return lookup_7Eaaxx(h8300h);
    // case 0x0f: return lookup_7Faaxx(h8300h);
    default:   return nullptr;
    }
}

instruction_parser_t lookup_74(H8Board* h8300h)
{
    // unsigned char b1 = h8300h->fetch_instruction_byte(1);
    // unsigned char bh_msb = (b1 & 0x80) == 0;

    return nullptr;
    // return bh_msb ? nullptr  // BOR
    //               : nullptr; // BIOR
}

instruction_parser_t lookup_75(H8Board* h8300h)
{
    // unsigned char b1 = h8300h->fetch_instruction_byte(1);
    // unsigned char bh_msb = (b1 & 0x80) == 0;

    return nullptr;
    // return bh_msb ? nullptr  // BXOR
    //               : nullptr; // BIXOR
}

instruction_parser_t lookup_76(H8Board* h8300h)
{
    // unsigned char b1 = h8300h->fetch_instruction_byte(1);
    // unsigned char bh_msb = (b1 & 0x80) == 0;

    return nullptr;
    // return bh_msb ? nullptr  // BAND
    //               : nullptr; // BIAND
}

instruction_parser_t lookup_77(H8Board* h8300h)
{
    // unsigned char b1 = h8300h->fetch_instruction_byte(1);
    // unsigned char bh_msb = (b1 & 0x80) == 0;

    return nullptr;
    // return bh_msb ? h8instructions::bld::bld_register_direct
    //               : nullptr; // BILD
}

instruction_parser_t lookup_79(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov::immediate_w_parse;
    case 0x01: return h8instructions::add::add_immediate_w_parse;
    case 0x02: return h8instructions::cmp::cmp_immediate_w_parse;
    // case 0x03: return nullptr; // SUB
    case 0x04: return h8instructions::orl::immediate_w_parse;
    // case 0x05: return nullptr; // XOR
    case 0x06: return h8instructions::andl::and_immediate_w_parse;
    default:   return nullptr;
    }
}

instruction_parser_t lookup_7A(H8Board* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov::immediate_l_parse;
    case 0x01: return h8instructions::add::add_immediate_l_parse;
    case 0x02: return h8instructions::cmp::cmp_immediate_l_parse;
    // case 0x03: return h8instructions::sub::sub_immediate_l;
    // case 0x04: return nullptr; // OR
    // case 0x05: return nullptr; // XOR
    case 0x06: return h8instructions::andl::and_immediate_l_parse;
    default:   return nullptr;
    }
}

}
