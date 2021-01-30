#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <mutex>
#include <queue>
#include "sci_register.h"
#include "interrupt/interrupt_controller.h"

class SCI {
    static const interrupt_t TXI_TABLE[3];
    static const interrupt_t RXI_TABLE[3];

public:
    typedef enum {
        TX,
        RX
    } DIRECTION;

private:
    std::thread* prepare_thread;
    std::thread* sci_thread[2];
    bool use_stdio;
    char sci_sock_name[16];
    int sci_socket;
    int sci_sock_fd;

    uint8_t index;
    bool terminate_flag;
    std::mutex& mutex;
    SCIRegister sci_register;
    // InterruptController& interrupt_controller;

    bool hasTxiInterruption;
    bool hasRxiInterruption;

    bool open_sci_socket();

    void prepare();
    void run_recv_from_h8();
    void run_send_to_h8();

public:
    SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex, bool use_stdio = false);
    ~SCI();

    void run();
    void terminate();

    interrupt_t getInterrupt();
    void clearInterrupt(interrupt_t type);

    void dump(FILE* fp);

public:
    // H8 のアプリコードからアクセスされる
    uint8_t read(uint32_t address);
    void write(uint32_t address, uint8_t value);

};

#endif
