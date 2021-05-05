#include "operation_map_7Dx.h"
#include "instructions/bset.h"

namespace operation_map {

instruction_handler_t lookup_7Dr067x(H8Board* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_handler_t lookup_7Dr0xx(H8Board* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);

    switch (b2) {
    case 0x60: return nullptr; // BSET
    case 0x61: return nullptr; // BNOT
    case 0x62: return nullptr; // BCLR
    case 0x67: return lookup_7Dr067x(h8300h);
    case 0x70: return h8instructions::bset::bset_register_indirect;
    case 0x71: return nullptr; // BNOT
    case 0x72: return nullptr; // BCLR
    default:   return nullptr;
    }
}

}
