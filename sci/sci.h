#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include "sci_register.h"

class Sci {
public:
    static void start(uint8_t index, InnerMemory& memory, bool& terminate);

private:
    uint8_t index;
    bool& terminate;
    SciRegister sci_register;

    Sci(uint8_t index, InnerMemory& memory, bool& terminate);
    bool send_requested();
    void process_send_request();

public:
    void run();

};

#endif
