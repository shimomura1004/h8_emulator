#include "operation_map_7Fx.h"

namespace operation_map2 {

instruction_parser_t lookup_7Faa67x(H8Board* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_parser_t lookup_7Faaxx(H8Board* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char ch = (b2 & 0xf0) >> 4;
    unsigned char cl = b2 & 0x0f;

    switch (ch) {
    case 0x06:
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        case 0x07: return lookup_7Faa67x(h8300h);
        default:   return nullptr;
        };
    case 0x07: 
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        default:   return nullptr;
        };
    default: return nullptr;
    }
}

}
