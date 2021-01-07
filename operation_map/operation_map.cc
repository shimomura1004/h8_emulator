#include "operation_map.h"
#include "operation_map_0x.h"
#include "operation_map_1x.h"
#include "operation_map_4x.h"
#include "operation_map_5x.h"
#include "operation_map_6x.h"
#include "operation_map_7x.h"

#include "../instructions/mov.h"
#include "../instructions/cmp.h"
#include "../instructions/and.h"

instruction_handler_t OperationMap::lookup(H8300H* h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char ah = (b0 & 0xf0) >> 4;

    switch (ah) {
    case 0x00: return lookup_0x(h8300h);
    case 0x01: return lookup_1x(h8300h);
    case 0x02:
    case 0x03: return nullptr; // MOV.B
    case 0x04: return lookup_4x(h8300h);
    case 0x05: return lookup_5x(h8300h);
    case 0x06: return lookup_6x(h8300h);
    case 0x07: return lookup_7x(h8300h);
    case 0x08: return nullptr; // ADD
    case 0x09: return nullptr; // ADDX
    case 0x0a: return h8instructions::cmp::cmp_b;
    case 0x0b: return nullptr; // SUBX
    case 0x0c: return nullptr; // OR
    case 0x0d: return nullptr; // XOR
    case 0x0e: return h8instructions::andl::and_immediate_b;
    case 0x0f: return h8instructions::mov::mov;
    default:   return nullptr;
    }
}