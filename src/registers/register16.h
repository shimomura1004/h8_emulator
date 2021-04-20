#ifndef _REGISTER_16_INCLUDED_
#define _REGISTER_16_INCLUDED_

#include <cstdint>
#include "register32.h"

// R(0~7) と E(8~F) という対応
class Register16 {
    Register32& reg;
    uint8_t index;

public:
    Register16(Register32& reg, uint8_t index) : reg(reg), index(index) {}

    uint16_t get() const {
        return (this->index < 8) ? this->reg.get_r() : this->reg.get_e();
    }
    void set(uint16_t value) {
        (this->index < 8) ? this->reg.set_r(value) : this->reg.set_e(value);
    }

};

#endif
