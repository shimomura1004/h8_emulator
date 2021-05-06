#ifndef _H8300H_SCI_INCLUDED_
#define _H8300H_SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <queue>
#include "interrupt/interrupt_type.h"
#include "sci.h"
#include "adm3202_registers.h"

class ADM3202 : public ISCI {
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
    ADM3202_Registers sci_registers;

    std::condition_variable& interrupt_cv;

    bool hasTxiInterruption;
    bool hasRxiInterruption;

    bool open_sci_socket();

    void prepare();
    void run_recv_from_h8();
    void run_send_to_h8();

public:
    ADM3202(uint8_t index, std::condition_variable& interrupt_cv, bool use_stdio = false);
    ~ADM3202() override;

    void run() override;
    void terminate() override;

    interrupt_t getInterrupt() override;
    void clearInterrupt(interrupt_t type) override;

    void dump(FILE* fp) override;

public:
    // H8 のアプリコードからアクセスされる
    uint8_t read(uint32_t address) override;
    void write(uint32_t address, uint8_t value) override;

};

#endif
