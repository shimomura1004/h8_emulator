#ifndef _RUNNER_INCLUDED_
#define _RUNNER_INCLUDED_

#include <string>
#include <set>
#include "h8300h.h"

class Runner {
    H8300H& h8;
    std::set<uint32_t> breakpoints;
    std::vector<uint32_t> call_stack;

    bool load_file_to_memory(uint32_t address, char *filename);

    void print_help_command();
    void set_breakpoint_command(char *buf);
    void write_value_command(char *buf);

public:
    Runner(H8300H& h8) : h8(h8) {}

    int proccess_debugger_command();
    void run(bool debug = false);

};

#endif
