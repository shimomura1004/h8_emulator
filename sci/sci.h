#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include "sci_register.h"

class Sci {
public:
    static void start(uint8_t index, InnerMemory& memory, bool& terminate);

private:
    uint8_t index;
    bool& terminate;
    SciRegister sci_register;

    fd_set fdset;
	struct timeval timeout;

    Sci(uint8_t index, InnerMemory& memory, bool& terminate);
    bool send_requested();
    void process_recv_request();
    void process_send_request();

public:
    void run();

};

#endif
