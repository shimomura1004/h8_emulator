#ifndef _RTL8019AS_INCLUDED_
#define _RTL8019AS_INCLUDED_

#include <cstdio>
#include <cstdint>
#include "tap_device.h"

class RTL8019AS {
    // page0, read
    uint8_t CR;
    uint8_t CLDA0;
    uint8_t CLDA1;
    uint8_t BNRY;
    uint8_t TSR;
    uint8_t NCR;
    uint8_t FIFO;
    uint8_t ISR;
    uint8_t CRDA0;
    uint8_t CRDA1;
    uint8_t CHIP8019ID0;
    uint8_t CHIP8019ID1;
    uint8_t RSR;
    uint8_t CNTR0;
    uint8_t CNTR1;
    uint8_t CNTR2;

    // page0, write
    uint8_t PSTART;
    uint8_t PSTOP;
    uint8_t TPSR;
    uint8_t TBCR0;
    uint8_t TBCR1;
    uint8_t RSAR0;
    uint8_t RSAR1;
    uint8_t RBCR0;
    uint8_t RBCR1;
    uint8_t RCR;
    uint8_t TCR;
    uint8_t DCR;
    uint8_t IMR;

    // page1, read/write
    uint8_t PAR0;
    uint8_t PAR1;
    uint8_t PAR2;
    uint8_t PAR3;
    uint8_t PAR4;
    uint8_t PAR5;
    uint8_t CURR;
    uint8_t MAR0;
    uint8_t MAR1;
    uint8_t MAR2;
    uint8_t MAR3;
    uint8_t MAR4;
    uint8_t MAR5;
    uint8_t MAR6;
    uint8_t MAR7;

    // page2
    // page3, read
    uint8_t CR9346CR;
    uint8_t BPAGE;
    uint8_t CONFIG0;
    uint8_t CONFIG1;
    uint8_t CONFIG2;
    uint8_t CONFIG3;
    uint8_t CSNSAV;
    uint8_t INTR;
    uint8_t CONFIG4;

    // page3, write
    uint8_t TEST;
    uint8_t HLTCLK;
    uint8_t FMWP;

    uint8_t* const register_table[4][2][16] = {
        {
            {
                &CR, &CLDA0, &CLDA1, &BNRY, &TSR, &NCR, &FIFO, &ISR,
                &CRDA0, &CRDA1, &CHIP8019ID0, &CHIP8019ID1, &RSR, &CNTR0, &CNTR1, &CNTR2,
            },
            {
                &CR, &PSTART, &PSTOP, &TPSR, &BNRY, &TBCR0, &TBCR1, &ISR,
                &RSAR0, &RSAR1, &RBCR0, &RBCR1, &RCR, &TCR, &DCR, &IMR, 
            }
        },
        {
            {
                &CR, &PAR0, &PAR1, &PAR2, &PAR3, &PAR4, &PAR5, &CURR,
                &MAR0, &MAR1, &MAR2, &MAR3, &MAR4, &MAR5, &MAR6, &MAR7,
            },
            {
                &CR, &PAR0, &PAR1, &PAR2, &PAR3, &PAR4, &PAR5, &CURR,
                &MAR0, &MAR1, &MAR2, &MAR3, &MAR4, &MAR5, &MAR6, &MAR7,
            }
        },
        {
            {
                &CR, &PSTART, &PSTOP, nullptr, &TPSR, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, &RCR, &TCR, &DCR, &IMR,
            },
            {
                &CR, &PSTART, &PSTOP, nullptr, &TPSR, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, &RCR, &TCR, &DCR, &IMR,
            },
        },
        {
            {
                &CR, &CR9346CR, &BPAGE, &CONFIG0, &CONFIG1, &CONFIG2, &CONFIG3, nullptr,
                &CSNSAV, nullptr, nullptr, &INTR, nullptr, &CONFIG4, nullptr, nullptr,
            },
            {
                &CR, &CR9346CR, &BPAGE, nullptr, &CONFIG1, &CONFIG2, &CONFIG3, &TEST,
                nullptr, &HLTCLK, nullptr, nullptr, &FMWP,  nullptr, nullptr, nullptr,
            }
        }
    };

    TAPDevice tap_device;

    uint8_t getPage();
    uint8_t* getRegister(uint8_t index, uint8_t page, bool read_mode);

public:
    RTL8019AS();

    uint8_t read8(uint32_t address);
    // uint16_t read16(uint32_t address);
    // uint32_t read32(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    // void write16(uint32_t address, uint16_t value);
    // void write32(uint32_t address, uint32_t value);

    void dump(FILE* fp);

};

#endif
