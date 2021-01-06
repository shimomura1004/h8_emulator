#include "operation_map_4x.h"

instruction_handler_t lookup_4x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // BRA
    case 0x01: return nullptr; // BRN
    case 0x02: return nullptr; // BHI
    case 0x03: return nullptr; // BLS
    case 0x04: return nullptr; // BCC
    case 0x05: return nullptr; // BCS
    case 0x06: return nullptr; // BNE
    case 0x07: return nullptr; // BEQ
    case 0x08: return nullptr; // BVC
    case 0x09: return nullptr; // BVS
    case 0x0a: return nullptr; // BPL
    case 0x0b: return nullptr; // BMI
    case 0x0c: return nullptr; // BGE
    case 0x0d: return nullptr; // BLT
    case 0x0e: return nullptr; // BGT
    case 0x0f: return nullptr; // BLE
    default:   return nullptr;
    }
}
