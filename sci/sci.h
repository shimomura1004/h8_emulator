#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <mutex>
#include <queue>
#include "sci_register.h"
#include "interrupt/interrupt_controller.h"

class Sci {
private:
    std::thread* sci_thread;

    uint8_t index;
    bool& terminate;
    std::mutex& mutex;
    SciRegister sci_register;
    InterruptController& interrupt_controller;

    std::queue<char> buffer;

    bool send_requested();
    void process_recv_request();
    void process_send_request();

public:
    Sci(uint8_t index, InnerMemory& memory, InterruptController& interrupt_controller, bool& terminate, std::mutex& mutex);
    ~Sci();
    void run();

};

#endif
