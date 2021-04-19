#include "operation_map_0x.h"
#include "instructions/mov.h"
#include "instructions/mov_register_direct.h"
#include "instructions/mov_register_indirect.h"
#include "instructions/mov_register_indirect_displacement.h"
#include "instructions/mov_absolute_address.h"
#include "instructions/adds.h"
#include "instructions/add.h"
#include "instructions/mulxs.h"
#include "instructions/nop.h"
#include "instructions/orc.h"
#include "instructions/andc.h"
#include "instructions/sleep.h"
#include "instructions/xor.h"
#include "instructions/divxs.h"

instruction_handler_t lookup_0100(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    switch (b2) {
    case 0x69: return h8instructions::mov::register_indirect_l;
    case 0x6b: return h8instructions::mov::absolute_address_24_l;
    case 0x6d: return h8instructions::mov::register_indirect_with_increment_decrement_l;
    case 0x6f: return h8instructions::mov::register_indirect_with_displacement16_l;
    case 0x78: return h8instructions::mov::register_indirect_with_displacement24_l;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01C05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x00: return h8instructions::mulxs::mulxs_b;
    case 0x02: return h8instructions::mulxs::mulxs_w;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01D05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x01: return nullptr; // DIVXS
    case 0x03: return h8instructions::divxs::divxs_w;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01F06x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x04: return nullptr; // OR
    case 0x05: return h8instructions::xorl::xor_register_direct_l;
    case 0x06: return nullptr; // AND
    default:   return nullptr;
    }
}

instruction_handler_t lookup_01(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return lookup_0100(h8300h);
    case 0x04: return nullptr; // LDC/STC
    case 0x08: return h8instructions::sleep::sleep;
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
    case 0x0f: return h8instructions::add::add_register_direct_l;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_0B(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::adds::adds;
    case 0x05: return nullptr; // INC
    case 0x08:
    case 0x09: return h8instructions::adds::adds;
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
    case 0x0f: return h8instructions::mov::register_direct_l;
    default:   return nullptr;
    }
}

instruction_handler_t lookup_0x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return h8instructions::nop::nop;
    case 0x01: return lookup_01(h8300h);
    case 0x02: return nullptr; // STC
    case 0x03: return nullptr; // LDC
    case 0x04: return h8instructions::orc::orc;
    case 0x05: return nullptr; // XORC
    case 0x06: return h8instructions::andc::andc;
    case 0x07: return nullptr; // LDC
    case 0x08: return h8instructions::add::add_register_direct_b;
    case 0x09: return h8instructions::add::add_register_direct_w;
    case 0x0a: return lookup_0A(h8300h);
    case 0x0b: return lookup_0B(h8300h);
    case 0x0c: return h8instructions::mov::register_direct_b;
    case 0x0d: return h8instructions::mov::register_direct_w;
    case 0x0e: return nullptr; // ADDX
    case 0x0f: return lookup_0F(h8300h);
    default:   return nullptr;
    }
}
