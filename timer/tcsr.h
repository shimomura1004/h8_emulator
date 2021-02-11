#ifndef _TIMER_TCSR_INCLUDED_
#define _TIMER_TCSR_INCLUDED_

#include <cstdint>

class TCSR {
    uint8_t reg;

public:
    TCSR(uint8_t channel) : reg(channel != 2 ? 0x00 : 0x10) {}

    uint8_t get_raw();
    void set_raw(uint8_t value);

    bool get_tcsr_cmfb();
    bool get_tcsr_cmfa();
    bool get_tcsr_ovf();
    bool get_tcsr_adte_ice();
    uint8_t get_tcsr_ois();

    void set_tcsr_cmfb(bool b);
    void set_tcsr_cmfa(bool b);
    void set_tcsr_ovf(bool b);

};

#endif
