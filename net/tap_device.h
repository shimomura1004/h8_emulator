#ifndef _TAP_DEVICE_INCLUDED_
#define _TAP_DEVICE_INCLUDED_

#include <stdint.h>

class TAPDevice {
    static const uint8_t DEVICE_NAME_SIZE = 8;

    char device_name[DEVICE_NAME_SIZE];
    int device_fd;

public:
    TAPDevice(const char *device_name);
    ~TAPDevice();

    bool createDevice();

    uint16_t read(char *buffer, uint16_t size);
    uint16_t write(const char *buffer, uint16_t size);

};

#endif
