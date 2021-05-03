#ifndef _REGISTER_8_INCLUDED_
#define _REGISTER_8_INCLUDED_

#include <cstdint>
#include "register32.h"

class Register8 {
    Register32& reg;
    uint8_t index;

public:
    Register8(Register32& reg, uint8_t index) : reg(reg), index(index) {}

    uint8_t get() const {
        return (this->index < 8) ? this->reg.get_rh() : this->reg.get_rl();
    }
    void set(uint8_t value) {
        (this->index < 8) ? this->reg.set_rh(value) : this->reg.set_rl(value);
    }

};

#endif
