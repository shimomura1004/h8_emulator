#include "tcr.h"
#include <cstdio>

#define H8_3069F_TMR_TCR_CKS          (0b00000111)
#define H8_3069F_TMR_TCR_DISCLK       (0<<0)
#define H8_3069F_TMR_TCR_CLK8         (1<<0)
#define H8_3069F_TMR_TCR_CLK64        (2<<0)
#define H8_3069F_TMR_TCR_CLK8192      (3<<0)
#define H8_3069F_TMR_TCR_OVF          (4<<0)
#define H8_3069F_TMR_TCR_CMFA         (4<<0)
#define H8_3069F_TMR_TCR_CLKUP        (5<<0)
#define H8_3069F_TMR_TCR_CLKDOWN      (6<<0)
#define H8_3069F_TMR_TCR_CLKBOTH      (7<<0)

#define H8_3069F_TMR_TCR_CCLR         (0b00011000)
#define H8_3069F_TMR_TCR_CCLR_DISCLR  (0<<3)
#define H8_3069F_TMR_TCR_CCLR_CLRCMFA (1<<3)
#define H8_3069F_TMR_TCR_CCLR_CLRCMFB (2<<3)
#define H8_3069F_TMR_TCR_CCLR_DISINPB (3<<3)

#define H8_3069F_TMR_TCR_OVIE         (1<<5)
#define H8_3069F_TMR_TCR_CMIEA        (1<<6)
#define H8_3069F_TMR_TCR_CMIEB        (1<<7)

TCR::TCR()
    : reg(0x00)
{}

uint8_t TCR::get_raw()
{
    return this->reg;
}

void TCR::set_raw(uint8_t value)
{
    this->reg = value;
}

bool TCR::get_tcr_cmieb()
{
    return  this->reg & H8_3069F_TMR_TCR_CMIEB;
}

bool TCR::get_tcr_cmiea()
{
    return  this->reg & H8_3069F_TMR_TCR_CMIEA;
}

bool TCR::get_tcr_ovie()
{
    return  this->reg & H8_3069F_TMR_TCR_OVIE;
}

uint8_t TCR::get_tcr_cclr()
{
    return (this->reg & H8_3069F_TMR_TCR_CCLR) >> 3;
}

uint8_t TCR::get_tcr_cks()
{
    return  this->reg & H8_3069F_TMR_TCR_CKS;
}

void TCR::set_tcr_cmieb(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCR_CMIEB;
    } else {
        this->reg &= ~H8_3069F_TMR_TCR_CMIEB;
    }
}

void TCR::set_tcr_cmiea(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCR_CMIEA;
    } else {
        this->reg &= ~H8_3069F_TMR_TCR_CMIEA;
    }
}

void TCR::set_tcr_ovie(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCR_OVIE;
    } else {
        this->reg &= ~H8_3069F_TMR_TCR_OVIE;
    }
}

void TCR::set_tcr_cclr(uint8_t value)
{
    this->reg &= ~H8_3069F_TMR_TCR_CCLR;
    this->reg |= (value << 3) & H8_3069F_TMR_TCR_CCLR;
}

void TCR::set_tcr_cks(uint8_t value)
{
    this->reg &= ~H8_3069F_TMR_TCR_CKS;
    this->reg |= (value << 0) & H8_3069F_TMR_TCR_CKS;
}
