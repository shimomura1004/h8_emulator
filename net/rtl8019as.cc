#include "rtl8019as.h"

uint8_t RTL8019AS::getPage()
{
    return (this->CR & 0b11000000) >> 6;
}

uint8_t* RTL8019AS::getRegister(uint8_t index, uint8_t page, bool read_mode)
{
    return register_table[page][read_mode ? 0 : 1][index];
}

// todo: レジスタの初期値の設定
RTL8019AS::RTL8019AS()
    : CR(0x00)
    , CLDA0(0x00)
    , CLDA1(0x00)
    , BNRY(0x00)
    , TSR(0x00)
    , NCR(0x00)
    , FIFO(0x00)
    , ISR(0xff)
    , CRDA0(0x00)
    , CRDA1(0x00)
    , CHIP8019ID0(0x00)
    , CHIP8019ID1(0x00)
    , RSR(0x00)
    , CNTR0(0x00)
    , CNTR1(0x00)
    , CNTR2(0x00)
    , PSTART(0x00)
    , PSTOP(0x00)
    , TPSR(0x00)
    , TBCR0(0x00)
    , TBCR1(0x00)
    , RSAR0(0x00)
    , RSAR1(0x00)
    , RBCR0(0x00)
    , RBCR1(0x00)
    , RCR(0x00)
    , TCR(0x00)
    , DCR(0x00)
    , IMR(0x00)
    , PAR0(0x00)
    , PAR1(0x00)
    , PAR2(0x00)
    , PAR3(0x00)
    , PAR4(0x00)
    , PAR5(0x00)
    , CURR(0x00)
    , MAR0(0x00)
    , MAR1(0x00)
    , MAR2(0x00)
    , MAR3(0x00)
    , MAR4(0x00)
    , MAR5(0x00)
    , MAR6(0x00)
    , MAR7(0x00)
    , CR9346CR(0x00)
    , BPAGE(0x00)
    , CONFIG0(0x00)
    , CONFIG1(0x00)
    , CONFIG2(0x00)
    , CONFIG3(0x00)
    , CSNSAV(0x00)
    , INTR(0x00)
    , CONFIG4(0x00)
    , TEST(0x00)
    , HLTCLK(0x00)
    , FMWP(0x00)
{}

uint8_t RTL8019AS::read8(uint32_t address)
{
    if (address > 0x1f) {
        fprintf(stderr, "Error: Invalid access to RTL8019AS register (0x%x)", address);
        return 0;
    }

    uint8_t page = getPage();
    return *getRegister(address, page, true);
}

void RTL8019AS::write8(uint32_t address, uint8_t value)
{
}

void RTL8019AS::dump(FILE* fp)
{
}
