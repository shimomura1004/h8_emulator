#ifndef _RUNNER_INCLUDED_
#define _RUNNER_INCLUDED_

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include "board/h8board.h"
#include "debugger_parser.h"

class Debugger {
    H8Board& h8;
    std::set<uint32_t> breakpoints;
    std::vector<uint32_t> call_stack;
    DebuggerParser debugger_parser;

    bool load_file_to_memory(uint32_t address, char *filename);

    void set_breakpoint_command(uint32_t address);
    void write_value_command(char *buf);

public:
    Debugger(H8Board& h8) : h8(h8) {}

    int proccess_debugger_command();
    void run(bool debug = false);

};

#endif
