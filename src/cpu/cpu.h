#ifndef _CPU_INCLUDED_
#define _CPU_INCLUDED_

#include <condition_variable>
#include "cpu/h8register.h"
#include "cpu/ccr.h"

class ICPU {
public:
    virtual ~ICPU() {}

    virtual Register8& reg8(uint8_t index) = 0;
    virtual Register16& reg16(uint8_t index) = 0;
    virtual Register32& reg32(uint8_t index) = 0;
    virtual CCR& ccr() = 0;
    virtual Register32& sp() = 0;
    virtual uint32_t& pc() = 0;

    virtual std::condition_variable& get_interrupt_cv() = 0;
};

#endif
