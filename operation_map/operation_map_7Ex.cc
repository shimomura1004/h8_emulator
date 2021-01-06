#include "operation_map_7Ex.h"

instruction_handler_t lookup_7Eaa74x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BOR
                  : nullptr; // BIOR
}

instruction_handler_t lookup_7Eaa75x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BXOR
                  : nullptr; // BIXOR
}

instruction_handler_t lookup_7Eaa76x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BAND
                  : nullptr; // BIAND
}

instruction_handler_t lookup_7Eaa77x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BLD
                  : nullptr; // BILD
}

instruction_handler_t lookup_7Eaaxx(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char ch = (b2 & 0xf0) >> 4;
    unsigned char cl = b2 & 0x0f;

    switch (ch) {
    case 0x06:
        switch (cl) {
        case 0x03: return nullptr; // BTST
        default:   return nullptr;
        };
    case 0x07: 
        switch (cl) {
        case 0x03: return nullptr; // BTST
        case 0x04: return lookup_7Eaa74x(h8300h);
        case 0x05: return lookup_7Eaa75x(h8300h);
        case 0x06: return lookup_7Eaa76x(h8300h);
        case 0x07: return lookup_7Eaa77x(h8300h);
        default:   return nullptr;
        }
    default: return nullptr;
    }
}
