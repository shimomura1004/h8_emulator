#ifndef _H8300H_CPU_INCLUDED_
#define _H8300H_CPU_INCLUDED_

#include "cpu.h"
#include "h8register.h"
#include "ccr.h"

class H8300H_CPU : public ICPU {
    Register32 _reg[8];
    Register32& _sp;
    Register16 _reg16[16];
    Register8 _reg8[16];
    CCR _ccr;
    uint32_t _pc;

public:
    H8300H_CPU();

    virtual Register8& reg8(uint8_t index) override;
    virtual Register16& reg16(uint8_t index) override;
    virtual Register32& reg32(uint8_t index) override;
    virtual CCR& ccr() override;
    virtual Register32& sp() override;
    virtual uint32_t& pc() override;

};

#endif
