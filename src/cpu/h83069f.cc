#include "h83069f.h"

H83069F::H83069F()
    : _sp(_reg[7])
    , _reg16{ Register16(_reg[0],  0), Register16(_reg[1],  1), Register16(_reg[2],  2), Register16(_reg[3],  3),
              Register16(_reg[4],  4), Register16(_reg[5],  5), Register16(_reg[6],  6), Register16(_reg[7],  7),
              Register16(_reg[0],  8), Register16(_reg[1],  9), Register16(_reg[2], 10), Register16(_reg[3], 11),
              Register16(_reg[4], 12), Register16(_reg[5], 13), Register16(_reg[6], 14), Register16(_reg[7], 15) }
    ,  _reg8{ Register8(_reg[0],  0), Register8(_reg[1],  1), Register8(_reg[2],  2), Register8(_reg[3],  3),
              Register8(_reg[4],  4), Register8(_reg[5],  5), Register8(_reg[6],  6), Register8(_reg[7],  7),
              Register8(_reg[0],  8), Register8(_reg[1],  9), Register8(_reg[2], 10), Register8(_reg[3], 11),
              Register8(_reg[4], 12), Register8(_reg[5], 13), Register8(_reg[6], 14), Register8(_reg[7], 15) }
    , _pc(0)
{}

Register8& H83069F::reg8(uint8_t index)
{
    return this->_reg8[index];
}

Register16& H83069F::reg16(uint8_t index)
{
    return this->_reg16[index];
}

Register32& H83069F::reg32(uint8_t index)
{
    return this->_reg[index];
}

CCR& H83069F::ccr()
{
    return this->_ccr;
}

Register32& H83069F::sp()
{
    return this->_sp;
}

uint32_t& H83069F::pc()
{
    return this->_pc;
}

std::condition_variable& H83069F::get_interrupt_cv()
{
    return this->interrupt_cv;
}
