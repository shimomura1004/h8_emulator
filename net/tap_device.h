#ifndef _TAP_DEVICE_INCLUDED_
#define _TAP_DEVICE_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include "interrupt/interrupt_type.h"

class TAPDevice {
    static const uint8_t DEVICE_NAME_SIZE = 8;

    uint8_t& BNRY;
    uint8_t& IMR;

    char device_name[DEVICE_NAME_SIZE];
    int device_fd;

    uint8_t saprom[(0x80 - 0x40 + 1) * 256];

    std::thread* prepare_thread;
    std::thread* tap_thread[2];

    bool terminate_flag;

    // todo: ISR レジスタが割込みの種別を持っている
    bool hasInterruption;
    std::condition_variable& interrupt_cv;

    void prepare();
    bool createDevice();

    void run_recv_from_tap();
    void run_send_to_tap();

public:
    TAPDevice(const char *device_name, std::condition_variable& interrupt_cv, uint8_t& BNRY, uint8_t& IMR);
    ~TAPDevice();

    void run();
    void terminate();

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    uint8_t dma_read(uint16_t address);
    void dma_write(uint16_t address, uint8_t value);

public:
    uint16_t read(char *buffer, uint16_t size);
    uint16_t write(const char *buffer, uint16_t size);

};

#endif
