#include <cstdio>
#include "ioport.h"

IOPort::IOPort()
    : p1ddr(0x00)
    , p1dr(0x00)
    , p2ddr(0x00)
    , p2dr(0x00)
    , p2pcr(0x00)
    , p3ddr(0x00)
    , p3dr(0x00)
    , p4ddr(0x00)
    , p4dr(0x00)
    , p4pcr(0x00)
    , p5ddr(0xf0)
    , p5dr(0xf0)
    , p5pcr(0xf0)
    , p6ddr(0x80)
    , p6dr(0x80)
    , p7dr(0x00)
    , p8ddr(0xe0)
    , p8dr(0xe0)
    , p9ddr(0xc0)
    , p9dr(0xc0)
    , paddr(0x00)
    , padr(0x00)
    , pbddr(0x00)
    , pbdr(0x00)
{}

uint8_t IOPort::read8(uint32_t address)
{
    switch (address) {
    case 0x00: return this->get_p1ddr();
    case 0x01: return this->get_p2ddr();
    case 0x02: return this->get_p3ddr();
    case 0x03: return this->get_p4ddr();
    case 0x04: return this->get_p5ddr();
    case 0x05: return this->get_p6ddr();
    case 0x07: return this->get_p8ddr();
    case 0x08: return this->get_p9ddr();
    case 0x09: return this->get_paddr();
    case 0x0a: return this->get_pbddr();
    default:
        fprintf(stderr, "Error: Invalid IO port read (%d)\n", address);
        return 0;
    }
}

void IOPort::write8(uint32_t address, uint8_t value)
{
    switch (address) {
    case 0x00:
        this->set_p1ddr(value);
        return;
    case 0x01:
        this->set_p2ddr(value);
        return;
    case 0x02:
        this->set_p3ddr(value);
        return;
    case 0x03:
        this->set_p4ddr(value);
        return;
    case 0x04:
        this->set_p5ddr(value);
        return;
    case 0x05:
        this->set_p6ddr(value);
        return;
    case 0x07:
        this->set_p8ddr(value);
        return;
    case 0x08:
        this->set_p9ddr(value);
        return;
    case 0x09:
        this->set_paddr(value);
        return;
    case 0x0a:
        this->set_pbddr(value);
        return;
    default:
        fprintf(stderr, "Error: Invalid IO port write (%d)\n", address);
    }
}
