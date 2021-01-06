#include "operation_map_7x.h"
#include "operation_map_7Cx.h"
#include "operation_map_7Dx.h"
#include "operation_map_7Ex.h"
#include "operation_map_7Fx.h"

#include "../instructions/mov.h"

instruction_handler_t lookup_7x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // BSET
    case 0x01: return nullptr; // BNOT
    case 0x02: return nullptr; // BCLR
    case 0x03: return nullptr; // BTST
    case 0x04: return lookup_74(h8300h);
    case 0x05: return lookup_75(h8300h);
    case 0x06: return lookup_76(h8300h);
    case 0x07: return lookup_77(h8300h);
    case 0x08: return h8instructions::mov::mov;
    case 0x09: return lookup_79(h8300h);
    case 0x0a: return lookup_7A(h8300h);
    case 0x0b: return nullptr; // EEPMOV
    case 0x0c: return lookup_7Cr0xx(h8300h);
    case 0x0d: return lookup_7Dr0xx(h8300h);
    case 0x0e: return lookup_7Eaaxx(h8300h);
    case 0x0f: return lookup_7Faaxx(h8300h);
    default:   return nullptr;
    }
}

instruction_handler_t lookup_74(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BOR
                  : nullptr; // BIOR
}

instruction_handler_t lookup_75(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BXOR
                  : nullptr; // BIXOR
}

instruction_handler_t lookup_76(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BAND
                  : nullptr; // BIAND
}

instruction_handler_t lookup_77(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BLD
                  : nullptr; // BILD
}

instruction_handler_t lookup_79(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov::mov;
    case 0x01: return nullptr; // ADD
    case 0x02: return nullptr; // CMP
    case 0x03: return nullptr; // SUB
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    default:   return nullptr;
    }
}

instruction_handler_t lookup_7A(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov::mov;
    case 0x01: return nullptr; // ADD
    case 0x02: return nullptr; // CMP
    case 0x03: return nullptr; // SUB
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    default:   return nullptr;
    }
}