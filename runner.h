#ifndef _RUNNER_INCLUDED_
#define _RUNNER_INCLUDED_

#include <set>
#include "h8300h.h"

class Runner {
    H8300H& h8;
    bool continue_mode;
    std::set<uint32_t> breakpoints;
    std::mutex& mutex;

    bool load_file_to_memory(uint32_t address, char *filename);

    void print_help_command();
    void set_breakpoint_command(char *buf);
    void load_file_command(char *buf);

public:
    Runner(H8300H& h8) : h8(h8), continue_mode(false), mutex(h8.get_mutex()) {}

    int proccess_debugger_command();
    void run(bool debug = false);

};

#endif
