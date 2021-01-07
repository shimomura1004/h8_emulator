#ifndef _RUNNER_INCLUDED_
#define _RUNNER_INCLUDED_

#include <set>
#include "h8300h.h"

class Runner {
    H8300H& h8;
    bool continue_mode;;
    std::set<uint32_t> breakpoints;

    void print_help();
    void set_breakpoint(char *buf);

public:
    Runner(H8300H& h8) : h8(h8), continue_mode(false) {}

    int proccess_debugger_command();
    void run(bool debug = false);

};

#endif
