#ifndef _SCI_REGISTER_INCLUDED_
#define _SCI_REGISTER_INCLUDED_

#include "inner_memory.h"

class SciRegister {
    const uint32_t sci_base_address[3] = { 0xffffb0, 0xffffb8, 0xffffc0 };
    uint32_t smr_address;
    uint32_t brr_address;
    uint32_t scr_address;
    uint32_t tdr_address;
    uint32_t ssr_address;
    uint32_t rdr_address;
    uint32_t scmr_address;

    InnerMemory& memory;

    bool get_scr_re();
    bool get_scr_te();
    bool get_ssr_tdre();
    void set_ssr_tdre(bool b);

    bool send_requested();

public:
    SciRegister(uint8_t index, InnerMemory& memory);
    void process_send_request();

};

#endif
