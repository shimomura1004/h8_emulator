#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <mutex>
#include <queue>
#include "sci_register.h"
#include "interrupt/interrupt_controller.h"

class SCI {
public:
    typedef enum {
        TX,
        RX
    } DIRECTION;

private:
    std::thread* sci_thread[2];
    char tx_pipe_name[16];
    char rx_pipe_name[16];
    FILE* tx;
    FILE* rx;

    uint8_t index;
    bool terminate_flag;
    std::mutex& mutex;
    SCIRegister sci_register;
    InterruptController& interrupt_controller;

    bool open_pipe(const char *pipe_name, FILE** fp, DIRECTION dir);

    void run_recv_from_h8();
    void run_send_to_h8();

public:
    SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex);
    SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex, FILE* tx, FILE* rx);
    ~SCI();

    void run();
    void terminate();

    void dump(FILE* fp);

public:
    // H8 のアプリコードからアクセスされる
    uint8_t read(uint32_t address);
    void write(uint32_t address, uint8_t value);

};

#endif
