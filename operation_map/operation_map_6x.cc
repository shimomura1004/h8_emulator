#include "operation_map_6x.h"
#include "instructions/mov.h"

instruction_handler_t lookup_67(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_handler_t lookup_6x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // BSET
    case 0x01: return nullptr; // BNOT
    case 0x02: return nullptr; // BCLR
    case 0x03: return nullptr; // BTST
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    case 0x07: return lookup_67(h8300h);
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return h8instructions::mov::mov;
    default:   return nullptr;
    }
}
