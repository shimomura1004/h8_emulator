#ifndef _TIMER_TCR_INCLUDED_
#define _TIMER_TCR_INCLUDED_

#include <cstdint>

class TCR {
    uint8_t reg;

public:
    TCR();

    uint8_t get_raw();
    void set_raw(uint8_t value);

    bool get_tcr_cmieb();
    bool get_tcr_cmiea();
    bool get_tcr_ovie();
    uint8_t get_tcr_cclr();
    uint8_t get_tcr_cks();

    void set_tcr_cmieb(bool b);
    void set_tcr_cmiea(bool b);
    void set_tcr_ovie(bool b);
    void set_tcr_cclr(uint8_t value);
    void set_tcr_cks(uint8_t value);

};

#endif
