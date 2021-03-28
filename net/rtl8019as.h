#ifndef _RTL8019AS_INCLUDED_
#define _RTL8019AS_INCLUDED_

#include <cstdio>
#include <cstdint>
#include "interrupt/interrupt_type.h"
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

    TAPDevice tap_device;

    uint8_t getPage();
    uint8_t* getRegister(uint8_t index, uint8_t page, bool read_mode);

public:
    RTL8019AS(std::condition_variable& interrupt_cv);

    uint8_t read8(uint32_t address);
    // uint16_t read16(uint32_t address);
    // uint32_t read32(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    // void write16(uint32_t address, uint16_t value);
    // void write32(uint32_t address, uint32_t value);

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    void dump(FILE* fp);

};

#endif
