#include "operation_map.h"
#include "instructions/mov.h"

instruction_handler_t OperationMap::lookup_01C05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x00: return nullptr; // MULXS
    case 0x02: return nullptr; // MULXS
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_01D05x(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char cl = b2 & 0x0f;
    switch (cl) {
    case 0x01: return nullptr; // DIVXS
    case 0x03: return nullptr; // DIVXS
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_01F06x(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_01(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov;
    case 0x04: return nullptr; // LDC/STC
    case 0x08: return nullptr; // SLEEP
    case 0x0c: return lookup_01C05x(h8300h);
    case 0x0d: return lookup_01D05x(h8300h);
    case 0x0f: return lookup_01F06x(h8300h);
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_0A(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_0B(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_0F(H8300H* h8300h)
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
    case 0x0f: return h8instructions::mov;
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_0x(H8300H *h8300h)
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
    case 0x0d: return h8instructions::mov;
    case 0x0e: return nullptr; // ADDX
    case 0x0f: return lookup_0F(h8300h);
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_10(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00:
    case 0x01: return nullptr; // SHLL
    case 0x03: return nullptr; // SHLL
    case 0x08:
    case 0x09: return nullptr; // SHAL
    case 0x0b: return nullptr; // SHAL
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_11(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_12(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_13(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_17(H8300H* h8300h)
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
    case 0x0d: return nullptr; // EXTS
    case 0x0f: return nullptr; // EXTS
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_1A(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_1B(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_1F(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_1x(H8300H *h8300h)
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
    case 0x08:
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

instruction_handler_t OperationMap::lookup_4x(H8300H *h8300h)
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

instruction_handler_t OperationMap::lookup_58(H8300H *h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
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

instruction_handler_t OperationMap::lookup_5x(H8300H *h8300h)
{
    unsigned char b0 = h8300h->fetch_instruction_byte(0);
    unsigned char al = b0 & 0x0f;

    switch (al) {
    case 0x00: return nullptr; // MULXU
    case 0x01: return nullptr; // DIVXU
    case 0x02: return nullptr; // MULXU
    case 0x03: return nullptr; // DIVXU
    case 0x04: return nullptr; // RTS
    case 0x05: return nullptr; // BSR
    case 0x06: return nullptr; // RTE
    case 0x07: return nullptr; // TRAPA
    case 0x08: return lookup_58(h8300h);
    case 0x09:
    case 0x0a:
    case 0x0b: return nullptr; // JMP
    case 0x0c: return nullptr; // BSR
    case 0x0d:
    case 0x0e:
    case 0x0f: return nullptr; // JSR
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_67(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_handler_t OperationMap::lookup_6x(H8300H *h8300h)
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
    case 0x0f: return h8instructions::mov;
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_74(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BOR
                  : nullptr; // BIOR
}

instruction_handler_t OperationMap::lookup_75(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BXOR
                  : nullptr; // BIXOR
}

instruction_handler_t OperationMap::lookup_76(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BAND
                  : nullptr; // BIAND
}

instruction_handler_t OperationMap::lookup_77(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh_msb = (b1 & 0x80) == 0;

    return bh_msb ? nullptr  // BLD
                  : nullptr; // BILD
}

instruction_handler_t OperationMap::lookup_79(H8300H* h8300h)
{
    unsigned char b1 = h8300h->fetch_instruction_byte(1);
    unsigned char bh = (b1 & 0xf0) >> 4;

    switch (bh) {
    case 0x00: return h8instructions::mov;
    case 0x01: return nullptr; // ADD
    case 0x02: return nullptr; // CMP
    case 0x03: return nullptr; // SUB
    case 0x04: return nullptr; // OR
    case 0x05: return nullptr; // XOR
    case 0x06: return nullptr; // AND
    default:   return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_7A(H8300H* h8300h)
{
    return lookup_79(h8300h);
}

instruction_handler_t OperationMap::lookup_7Cr074x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BOR
                  : nullptr; // BIOR
}

instruction_handler_t OperationMap::lookup_7Cr075x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BXOR
                  : nullptr; // BIXOR
}

instruction_handler_t OperationMap::lookup_7Cr076x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BAND
                  : nullptr; // BIAND
}

instruction_handler_t OperationMap::lookup_7Cr077x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BLD
                  : nullptr; // BILD
}

instruction_handler_t OperationMap::lookup_7Cr0xx(H8300H* h8300h)
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
        case 0x04: return lookup_7Cr074x(h8300h);
        case 0x05: return lookup_7Cr075x(h8300h);
        case 0x06: return lookup_7Cr076x(h8300h);
        case 0x07: return lookup_7Cr077x(h8300h);
        default:   return nullptr;
        }
    default: return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_7Dr067x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_handler_t OperationMap::lookup_7Dr0xx(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char ch = (b2 & 0xf0) >> 4;
    unsigned char cl = b2 & 0x0f;

    switch (ch) {
    case 0x06:
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        case 0x07: return lookup_7Dr067x(h8300h);
        default:   return nullptr;
        };
    case 0x07: 
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        default:   return nullptr;
        };
    default: return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_7Eaa74x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BOR
                  : nullptr; // BIOR
}

instruction_handler_t OperationMap::lookup_7Eaa75x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BXOR
                  : nullptr; // BIXOR
}

instruction_handler_t OperationMap::lookup_7Eaa76x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BAND
                  : nullptr; // BIAND
}

instruction_handler_t OperationMap::lookup_7Eaa77x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BLD
                  : nullptr; // BILD
}

instruction_handler_t OperationMap::lookup_7Eaaxx(H8300H* h8300h)
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

instruction_handler_t OperationMap::lookup_7Faa67x(H8300H* h8300h)
{
    unsigned char b3 = h8300h->fetch_instruction_byte(3);
    unsigned char dh_msb = (b3 & 0x80) == 0;

    return dh_msb ? nullptr  // BST
                  : nullptr; // BIST
}

instruction_handler_t OperationMap::lookup_7Faaxx(H8300H* h8300h)
{
    unsigned char b2 = h8300h->fetch_instruction_byte(2);
    unsigned char ch = (b2 & 0xf0) >> 4;
    unsigned char cl = b2 & 0x0f;

    switch (ch) {
    case 0x06:
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        case 0x07: return lookup_7Faa67x(h8300h);
        default:   return nullptr;
        };
    case 0x07: 
        switch (cl) {
        case 0x00: return nullptr; // BSET
        case 0x01: return nullptr; // BNOT
        case 0x02: return nullptr; // BCLR
        default:   return nullptr;
        };
    default: return nullptr;
    }
}

instruction_handler_t OperationMap::lookup_7x(H8300H *h8300h)
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
    case 0x08: return h8instructions::mov;
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
    case 0x0a: return nullptr; // CMP
    case 0x0b: return nullptr; // SUBX
    case 0x0c: return nullptr; // OR
    case 0x0d: return nullptr; // XOR
    case 0x0e: return nullptr; // AND
    case 0x0f: return h8instructions::mov;
    default:   return nullptr;
    }
}