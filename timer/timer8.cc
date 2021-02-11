#include "timer8.h"

Timer8::Timer8(std::condition_variable& interrupt_cv)
    : tmr8_0(0, tmr8_1, interrupt_cv)
    , tmr8_1(1, tmr8_0, interrupt_cv)
{
}

interrupt_t Timer8::getInterrupt()
{
    interrupt_t type = interrupt_t::NONE;

    type = tmr8_0.getInterrupt();
    if (type != interrupt_t::NONE) {
        return type;
    }

    return tmr8_1.getInterrupt();
}

void Timer8::clearInterrupt(interrupt_t type)
{
    // 複数のタイマから同じ割込みが発生した場合、
    // getInterrupt/clearInterrupt も割込みハンドラも
    // どのタイマから割込みが発生したか判断できない

    // 最初に見つかった割込みフラグをクリアする
    bool cleared = false;

    cleared = tmr8_0.clearInterrupt(type);
    if (cleared) {
        return;
    }

    tmr8_1.clearInterrupt(type);
}

uint8_t Timer8::read8(uint32_t address)
{
    switch (address) {
    case 0: return tmr8_0.get_tcr();
    case 1: return tmr8_1.get_tcr();
    case 2: return tmr8_0.get_tcsr();
    case 3: return tmr8_1.get_tcsr();
    case 4: return tmr8_0.get_tcora();
    case 5: return tmr8_1.get_tcora();
    case 6: return tmr8_0.get_tcorb();
    case 7: return tmr8_1.get_tcorb();
    case 8: return tmr8_0.get_tcnt();
    case 9: return tmr8_1.get_tcnt();
    default: 
        perror("not implemented Timer8::read8");
        return 0;
    }
}

uint16_t Timer8::read16(uint32_t address)
{
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint16_t low_value = read8(address);
    uint16_t high_value = read8(address + 1);
#else
    uint16_t high_value = read8(address);
    uint16_t low_value = read8(address + 1);
#endif
#endif

    return (high_value << 8) | low_value;
}

void Timer8::write8(uint32_t address, uint8_t value)
{
    // todo: switch 文は見た目が悪い…
    switch (address) {
    case 0:
        tmr8_0.set_tcr(value);
        break;
    case 1:
        tmr8_1.set_tcr(value);
        break;
    case 2:
        tmr8_0.set_tcsr(value);
        break;
    case 3:
        tmr8_1.set_tcsr(value);
        break;
    case 4:
        tmr8_0.set_tcora(value);
        break;
    case 5:
        tmr8_1.set_tcora(value);
        break;
    case 6:
        tmr8_0.set_tcorb(value);
        break;
    case 7:
        tmr8_1.set_tcorb(value);
        break;
    case 8:
        tmr8_0.set_tcnt(value);
        break;
    case 9:
        tmr8_1.set_tcnt(value);
        break;
    default:
        fprintf(stderr, "Error: Invalid TMR register access (%d)\n", address);
        break;
    }
}

void Timer8::write16(uint32_t address, uint16_t value)
{
    uint8_t high_value = value >> 8;
    uint8_t low_value = value & 0xff;

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    write8(address, low_value);
    write8(address + 1, high_value);
#else
    write8(address, high_value);
    write8(address + 1, low_value);
#endif
#endif
}
