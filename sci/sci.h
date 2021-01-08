#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <mutex>
#include <queue>
#include "sci_register.h"

class Sci {
    // const static int buffer_size = 256;

public:
    static void start(uint8_t index, InnerMemory& memory, bool& terminate, std::mutex& mutex);

private:
    uint8_t index;
    bool& terminate;
    std::mutex& mutex;
    SciRegister sci_register;

    fd_set fdset;
	struct timeval timeout;
    // char buffer[buffer_size];
    // uint16_t buffer_len;
    std::queue<char> buffer;

    Sci(uint8_t index, InnerMemory& memory, bool& terminate, std::mutex& mutex);
    bool send_requested();
    void process_recv_request();
    void process_send_request();

public:
    void run();

};

#endif
