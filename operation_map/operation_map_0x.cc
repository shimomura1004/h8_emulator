#include "operation_map_0x.h"
#include "../instructions/mov.h"

instruction_handler_t lookup_01C05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x00: return nullptr; // MULXS
    case 0x02: return nullptr; // MULXS
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01D05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x01: return nullptr; // DIVXS
    case 0x03: return nullptr; // DIVXS
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01F06x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov::mov;
    case 0x04: return nullptr; // LDC/STC
    case 0x08: return nullptr; // SLEEP
    case 0x0c: return lookup_01C05x(h8300h);
    case 0x0d: return lookup_01D05x(h8300h);
    case 0x0f: return lookup_01F06x(h8300h);
    default:   return nullptr;
    }
}

instruction_handler_t lookup_0A(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // INC
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return nullptr; // ADD
    default:   return nullptr;
    }
}

instruction_handler_t lookup_0B(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // ADDS
    case 0x05: return nullptr; // INC
    case 0x08:
    case 0x09: return nullptr; // ADDS
    case 0x0d: return nullptr; // INC
    case 0x0f: return nullptr; // INC
    default:   return nullptr;
    }
}

instruction_handler_t lookup_0F(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // DAA
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

instruction_handler_t lookup_0x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // NOP
    case 0x01: return lookup_01(h8300h);
    case 0x02: return nullptr; // STC
    case 0x03: return nullptr; // LDC
    case 0x04: return nullptr; // ORC
    case 0x05: return nullptr; // XORC
    case 0x06: return nullptr; // ANDC
    case 0x07: return nullptr; // LDC
    case 0x08:
    case 0x09: return nullptr; // ADD
    case 0x0a: return lookup_0A(h8300h);
    case 0x0b: return lookup_0B(h8300h);
    case 0x0c:
    case 0x0d: return h8instructions::mov::mov;
    case 0x0e: return nullptr; // ADDX
    case 0x0f: return lookup_0F(h8300h);
    default:   return nullptr;
    }
}