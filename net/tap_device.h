#ifndef _TAP_DEVICE_INCLUDED_
#define _TAP_DEVICE_INCLUDED_

#include <cstdint>
#include <thread>
#include "interrupt/interrupt_type.h"

class TAPDevice {
    static const uint8_t DEVICE_NAME_SIZE = 8;

    char device_name[DEVICE_NAME_SIZE];
    int device_fd;

    std::thread* prepare_thread;
    std::thread* tap_thread[2];

    bool terminate_flag;

    void prepare();
    bool createDevice();

    void run_recv_from_tap();
    void run_send_to_tap();

public:
    TAPDevice(const char *device_name);
    ~TAPDevice();

    void run();
    void terminate();

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

public:
    uint16_t read(char *buffer, uint16_t size);
    uint16_t write(const char *buffer, uint16_t size);

};

#endif
