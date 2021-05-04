#ifndef _H8300H_SCI_INCLUDED_
#define _H8300H_SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <queue>
#include "interrupt/interrupt_type.h"
#include "sci.h"
#include "h8300h_sci_registers.h"

class H8300H_SCI : public ISCI {
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
    H8300H_SCI_Registers sci_registers;

    std::condition_variable& interrupt_cv;

    bool hasTxiInterruption;
    bool hasRxiInterruption;

    bool open_sci_socket();

    void prepare();
    void run_recv_from_h8();
    void run_send_to_h8();

public:
    H8300H_SCI(uint8_t index, std::condition_variable& interrupt_cv, bool use_stdio = false);
    ~H8300H_SCI() override;

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
