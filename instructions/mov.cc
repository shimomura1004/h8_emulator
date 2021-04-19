#include "mov.h"

int h8instructions::mov::mov(H8300H* h8)
{
    uint8_t b0 = h8->fetch_instruction_byte(0);

    switch (b0) {
    case 0x01: {
        uint8_t b2 = h8->fetch_instruction_byte(2);
        switch (b2) {
        case 0x69: return -1;
        case 0x6b: return -1;
        case 0x6d: return -1;
        case 0x6f: return -1;
        case 0x78: return -1;
        default: return -1;
        }
    }
    case 0x0c: return -1;
    case 0x0d: return -1;
    case 0x0f: return -1;
    case 0x68: return -1;
    case 0x69: return -1;
    case 0x6a: return -1;
    case 0x6b: return -1;
    case 0x6c: return -1;
    case 0x6e: return -1;
    case 0x6f: return -1;
    case 0x78: return -1;
    case 0x79: return -1;
    case 0x7a: return -1;
    case 0xf0: case 0xf1: case 0xf2: case 0xf3:
    case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    case 0xf8: case 0xf9: case 0xfa: case 0xfb:
    case 0xfc: case 0xfd: case 0xfe: case 0xff:
        return -1;
    default:
        fprintf(stderr, "Unknown error in %s\n", __FILE__);
        return -1;
    }
}
