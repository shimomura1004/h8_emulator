#ifndef _BUSCONTROLLER_INCLUDED_
#define _BUSCONTROLLER_INCLUDED_

#include <cstdio>
#include <cstdint>

class BusController {
public:
    static const uint32_t abwcr_address;
    static const uint32_t astcr_address;
    static const uint32_t wcrh_address;
    static const uint32_t wcrl_address;
    static const uint32_t brcr_address;
    static const uint32_t cscr_address;
    static const uint32_t adrcr_address;
    static const uint32_t bcr_address;
    static const uint32_t drcra_address;
    static const uint32_t drcrb_address;
    static const uint32_t rtmcsr_address;
    static const uint32_t rtcnt_address;
    static const uint32_t rtcor_address;

private:
    uint8_t abwcr;
    uint8_t astcr;
    uint8_t wcrh;
    uint8_t wcrl;
    uint8_t brcr;
    uint8_t cscr;
    uint8_t adrcr;
    uint8_t bcr;
    uint8_t drcra;
    uint8_t drcrb;
    uint8_t rtmcsr;
    uint8_t rtcnt;
    uint8_t rtcor;

public:
    BusController();

    uint8_t read(uint32_t address);
    void write(uint32_t address, uint8_t value);

};

#endif
