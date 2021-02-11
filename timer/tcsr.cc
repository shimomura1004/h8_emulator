#include "tcsr.h"

#define H8_3069F_TMR_TCSR_OS_NOACT    (0<<0)
#define H8_3069F_TMR_TCSR_OIS_NOACT   (0<<2)
#define H8_3069F_TMR_TCSR_ADTE        (1<<4)
#define H8_3069F_TMR_TCSR_ICE         (1<<4)
#define H8_3069F_TMR_TCSR_OVF         (1<<5)
#define H8_3069F_TMR_TCSR_CMFA        (1<<6)
#define H8_3069F_TMR_TCSR_CMFB        (1<<7)

uint8_t TCSR::get_raw()
{
    return this->reg;
}

void TCSR::set_raw(uint8_t value)
{
    // todo: ビットごとに確認しつつ更新
    this->reg = value;
}

bool TCSR::get_tcsr_cmfb()
{
    return this->reg & H8_3069F_TMR_TCSR_CMFB;
}

bool TCSR::get_tcsr_cmfa()
{
    return this->reg & H8_3069F_TMR_TCSR_CMFA;
}

bool TCSR::get_tcsr_ovf()
{
    return this->reg & H8_3069F_TMR_TCSR_OVF;
}

bool TCSR::get_tcsr_adte_ice()
{
    return this->reg & H8_3069F_TMR_TCSR_ICE;
}

uint8_t TCSR::get_tcsr_ois()
{
    return this->reg & 0b00001111;
}

void TCSR::set_tcsr_cmfb(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCSR_CMFB;
    } else {
        this->reg &= ~H8_3069F_TMR_TCSR_CMFB;
    }
}

void TCSR::set_tcsr_cmfa(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCSR_CMFA;
    } else {
        this->reg &= ~H8_3069F_TMR_TCSR_CMFA;
    }
}

void TCSR::set_tcsr_ovf(bool b)
{
    if (b) {
        this->reg |= H8_3069F_TMR_TCSR_OVF;
    } else {
        this->reg &= ~H8_3069F_TMR_TCSR_OVF;
    }
}
