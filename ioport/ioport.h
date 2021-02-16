#ifndef _IOPORT_INCLUDED_
#define _IOPORT_INCLUDED_

#include <cstdint>

class IOPort {
    uint8_t p1ddr;
    uint8_t p1dr;

    uint8_t p2ddr;
    uint8_t p2dr;
    uint8_t p2pcr;

    uint8_t p3ddr;
    uint8_t p3dr;

    uint8_t p4ddr;
    uint8_t p4dr;
    uint8_t p4pcr;

    uint8_t p5ddr;
    uint8_t p5dr;
    uint8_t p5pcr;

    uint8_t p6ddr;
    uint8_t p6dr;

    uint8_t p7dr;

    uint8_t p8ddr;
    uint8_t p8dr;

    uint8_t p9ddr;
    uint8_t p9dr;

    uint8_t paddr;
    uint8_t padr;

    uint8_t pbddr;
    uint8_t pbdr;

public:
    IOPort();

    uint8_t read8(uint32_t address);
    void write8(uint32_t address, uint8_t value);

#define ACCESSOR(name) \
    uint8_t get_##name() { return this->name; } \
    void set_##name(uint8_t value) { this->name = value; }

    ACCESSOR(p1ddr)
    ACCESSOR(p1dr)

    ACCESSOR(p2ddr)
    ACCESSOR(p2dr)
    ACCESSOR(p2pcr)

    ACCESSOR(p3ddr)
    ACCESSOR(p3dr)

    ACCESSOR(p4ddr)
    ACCESSOR(p4dr)
    ACCESSOR(p4pcr)

    ACCESSOR(p5ddr)
    ACCESSOR(p5dr)
    ACCESSOR(p5pcr)

    ACCESSOR(p6ddr)
    ACCESSOR(p6dr)

    ACCESSOR(p7dr)

    ACCESSOR(p8ddr)
    ACCESSOR(p8dr)

    ACCESSOR(p9ddr)
    ACCESSOR(p9dr)

    ACCESSOR(paddr)
    ACCESSOR(padr)

    ACCESSOR(pbddr)
    ACCESSOR(pbdr)

};

#endif
