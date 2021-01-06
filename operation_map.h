#ifndef _OPERATION_MAP_INCLUDED_
#define _OPERATION_MAP_INCLUDED_

#include "h8300h.h"

typedef int(*instruction_handler_t)(H8300H*);

class OperationMap {
private:
    static instruction_handler_t lookup_0x(H8300H* h8300h);
    static instruction_handler_t lookup_01(H8300H* h8300h);
    static instruction_handler_t lookup_01C05x(H8300H* h8300h);
    static instruction_handler_t lookup_01D05x(H8300H* h8300h);
    static instruction_handler_t lookup_01F06x(H8300H* h8300h);
    static instruction_handler_t lookup_0A(H8300H* h8300h);
    static instruction_handler_t lookup_0B(H8300H* h8300h);
    static instruction_handler_t lookup_0F(H8300H* h8300h);

    static instruction_handler_t lookup_1x(H8300H* h8300h);
    static instruction_handler_t lookup_10(H8300H* h8300h);
    static instruction_handler_t lookup_11(H8300H* h8300h);
    static instruction_handler_t lookup_12(H8300H* h8300h);
    static instruction_handler_t lookup_13(H8300H* h8300h);
    static instruction_handler_t lookup_17(H8300H* h8300h);
    static instruction_handler_t lookup_1A(H8300H* h8300h);
    static instruction_handler_t lookup_1B(H8300H* h8300h);
    static instruction_handler_t lookup_1F(H8300H* h8300h);

    static instruction_handler_t lookup_4x(H8300H* h8300h);

    static instruction_handler_t lookup_5x(H8300H* h8300h);
    static instruction_handler_t lookup_58(H8300H* h8300h);

    static instruction_handler_t lookup_6x(H8300H* h8300h);
    static instruction_handler_t lookup_67(H8300H* h8300h);

    static instruction_handler_t lookup_7x(H8300H* h8300h);
    static instruction_handler_t lookup_74(H8300H* h8300h);
    static instruction_handler_t lookup_75(H8300H* h8300h);
    static instruction_handler_t lookup_76(H8300H* h8300h);
    static instruction_handler_t lookup_77(H8300H* h8300h);
    static instruction_handler_t lookup_79(H8300H* h8300h);
    static instruction_handler_t lookup_7A(H8300H* h8300h);

    static instruction_handler_t lookup_7Cr0xx(H8300H* h8300h);
    static instruction_handler_t lookup_7Cr074x(H8300H* h8300h);
    static instruction_handler_t lookup_7Cr075x(H8300H* h8300h);
    static instruction_handler_t lookup_7Cr076x(H8300H* h8300h);
    static instruction_handler_t lookup_7Cr077x(H8300H* h8300h);

    static instruction_handler_t lookup_7Dr0xx(H8300H* h8300h);
    static instruction_handler_t lookup_7Dr067x(H8300H* h8300h);

    static instruction_handler_t lookup_7Eaaxx(H8300H* h8300h);
    static instruction_handler_t lookup_7Eaa74x(H8300H* h8300h);
    static instruction_handler_t lookup_7Eaa75x(H8300H* h8300h);
    static instruction_handler_t lookup_7Eaa76x(H8300H* h8300h);
    static instruction_handler_t lookup_7Eaa77x(H8300H* h8300h);

    static instruction_handler_t lookup_7Faaxx(H8300H* h8300h);
    static instruction_handler_t lookup_7Faa67x(H8300H* h8300h);

public:
    static instruction_handler_t lookup(H8300H* h8300h);

};



#endif
