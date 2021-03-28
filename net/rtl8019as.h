#ifndef _RTL8019AS_INCLUDED_
#define _RTL8019AS_INCLUDED_

#include <cstdio>
#include <cstdint>
#include <thread>
#include <condition_variable>
#include "interrupt/interrupt_type.h"
#include "rtl8019as_register.h"

class RTL8019AS {
    static const uint8_t DEVICE_NAME_SIZE = 8;
    char device_name[DEVICE_NAME_SIZE];
    int device_fd;

    // todo: マジックナンバーをなくす
    uint8_t saprom[(0x80 - 0x40 + 1) * 256];
    RTL8019ASRegister rtl8019as_register;

    std::thread* prepare_thread;
    std::thread* tap_thread[2];
    bool terminate_flag;

    // todo: ISR レジスタが割込みの種別を持っている、細かくフラグをわけないといけない
    bool hasInterruption;
    std::condition_variable& interrupt_cv;

    void prepare();
    bool createDevice();

    void run_recv_from_tap();
    void run_send_to_tap();

public:
    RTL8019AS(std::condition_variable& interrupt_cv);
    ~RTL8019AS();

    void run();
    void terminate();

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    uint8_t dma_read(uint16_t address);
    void dma_write(uint16_t address, uint8_t value);

    void dump(FILE* fp);

public:
    uint8_t read8(uint32_t address);
    void write8(uint32_t address, uint8_t value);

};

#endif
