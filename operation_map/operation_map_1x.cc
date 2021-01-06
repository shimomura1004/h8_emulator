#include "operation_map_1x.h"
#include "../instructions/exts.h"
#include "../instructions/shll.h"
#include "../instructions/sub.h"

instruction_handler_t lookup_10(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::shll::shll_b;
    case 0x01: return h8instructions::shll::shll_w;
    case 0x03: return h8instructions::shll::shll_l;
    case 0x08:
    case 0x09: return nullptr; // SHAL
    case 0x0b: return nullptr; // SHAL
    default:   return nullptr;
    }
}

instruction_handler_t lookup_11(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00:
    case 0x01: return nullptr; // SHLR
    case 0x03: return nullptr; // SHLR
    case 0x08:
    case 0x09: return nullptr; // SHAR
    case 0x0b: return nullptr; // SHAR
    default:   return nullptr;
    }
}

instruction_handler_t lookup_12(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00:
    case 0x01: return nullptr; // ROTXL
    case 0x03: return nullptr; // ROTXL
    case 0x08:
    case 0x09: return nullptr; // ROTL
    case 0x0b: return nullptr; // ROTL
    default:   return nullptr;
    }
}

instruction_handler_t lookup_13(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00:
    case 0x01: return nullptr; // ROTXR
    case 0x03: return nullptr; // ROTXR
    case 0x08:
    case 0x09: return nullptr; // ROTR
    case 0x0b: return nullptr; // ROTR
    default:   return nullptr;
    }
}

instruction_handler_t lookup_17(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00:
    case 0x01: return nullptr; // NOT
    case 0x03: return nullptr; // NOT
    case 0x05: return nullptr; // EXTU
    case 0x07: return nullptr; // EXTU
    case 0x08:
    case 0x09: return nullptr; // NEG
    case 0x0b: return nullptr; // NEG
    case 0x0d: return h8instructions::exts::exts_w;
    case 0x0f: return h8instructions::exts::exts_l;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_1A(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // DEC
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return nullptr; // SUB
    default:   return nullptr;
    }
}

instruction_handler_t lookup_1B(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // SUBS
    case 0x05: return nullptr; // DEC
    case 0x07: return nullptr; // DEC
    case 0x08:
    case 0x09: return nullptr; // SUBS
    case 0x0d: return nullptr; // DEC
    case 0x0f: return nullptr; // DEC
    default:   return nullptr;
    }
}

instruction_handler_t lookup_1F(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return nullptr; // DAS
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f: return nullptr; // CMP
    default:   return nullptr;
    }
}

instruction_handler_t lookup_1x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return lookup_10(h8300h);
    case 0x01: return lookup_11(h8300h);
    case 0x02: return lookup_12(h8300h);
    case 0x03: return lookup_13(h8300h);
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    case 0x07: return lookup_17(h8300h);
    case 0x08: return h8instructions::sub::sub_b;
    case 0x09: return nullptr; // SUB
    case 0x0a: return lookup_1A(h8300h);
    case 0x0b: return lookup_1B(h8300h);
    case 0x0c:
    case 0x0d: return nullptr; // CMP
    case 0x0e: return nullptr; // SUBX 
    case 0x0f: return lookup_1F(h8300h);
    default:   return nullptr;
    }
}