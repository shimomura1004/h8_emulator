#ifndef _RTL8019AS_INCLUDED_
#define _RTL8019AS_INCLUDED_

#include <cstdint>
#include <thread>
#include <condition_variable>
#include "nic.h"
#include "rtl8019as_register.h"

// todo: ping への応答が1秒近くかかっている

class RTL8019AS : public INIC {
    static const uint8_t DEVICE_NAME_SIZE = 8;
    char device_name[DEVICE_NAME_SIZE];
    int device_fd;

    // SAPROM には 256 バイトのページが 0x80 個ある
    uint8_t saprom[0x80 * 256];
    RTL8019ASRegister rtl8019as_register;

    std::thread* prepare_thread;
    std::thread* tap_thread[2];
    bool terminate_flag;

    // todo: ISR レジスタが割込みの種別を持っている、細かくフラグをわけないといけない
    bool hasInterruption;
    std::condition_variable& interrupt_cv;

    void prepare();

    void run_recv_from_tap();
    void run_send_to_tap();

public:
    RTL8019AS(std::condition_variable& interrupt_cv);
    virtual ~RTL8019AS() override;

    virtual void run() override;
    virtual void terminate() override;

    virtual interrupt_t getInterrupt() override;
    virtual void clearInterrupt(interrupt_t type) override;

    virtual uint8_t dma_read(uint16_t address) override;
    virtual void dma_write(uint16_t address, uint8_t value) override;

    virtual void dump(FILE* fp) override;

public:
    virtual uint8_t read8(uint32_t address) override;
    virtual void write8(uint32_t address, uint8_t value) override;

};

#endif
