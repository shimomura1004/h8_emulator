#include "operation_map.h"
#include "operation_map_0x.h"
#include "operation_map_1x.h"
#include "operation_map_4x.h"
#include "operation_map_5x.h"
#include "operation_map_6x.h"
#include "operation_map_7x.h"

#include "instructions/mov_immediate.h"
#include "instructions/cmp.h"
#include "instructions/and.h"
#include "instructions/add.h"
#include "instructions/xor.h"
#include "instructions/or.h"

namespace operation_map {

instruction_handler_t lookup(H8Board* h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char ah = (b0 & 0xf0) >> 4;

    switch (ah) {
    case 0x00: return lookup_0x(h8300h);
    case 0x01: return lookup_1x(h8300h);
    case 0x06: return lookup_6x(h8300h);
    case 0x07: return lookup_7x(h8300h);
    default: return nullptr;
    }
}

}

namespace operation_map2 {

instruction_parser_t lookup(H8Board* h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char ah = (b0 & 0xf0) >> 4;

    switch (ah) {
    case 0x00: return lookup_0x(h8300h);
    case 0x01: return lookup_1x(h8300h);
    // case 0x02:
    // case 0x03: return nullptr; // MOV.B
    case 0x04: return lookup_4x(h8300h);
    case 0x05: return lookup_5x(h8300h);
    case 0x06: return lookup_6x(h8300h);
    case 0x07: return lookup_7x(h8300h);
    case 0x08: return h8instructions::add::add_immediate_b_parse;
    // case 0x09: return nullptr; // ADDX
    case 0x0a: return h8instructions::cmp::cmp_immediate_b_parse;
    // case 0x0b: return nullptr; // SUBX
    case 0x0c: return h8instructions::orl::immediate_b_parse;
    case 0x0d: return h8instructions::xorl::immediate_b_parse;
    case 0x0e: return h8instructions::andl::and_immediate_b_parse;
    case 0x0f: return h8instructions::mov::immediate_b_parse;
    default:   return nullptr;
    }
}

}
