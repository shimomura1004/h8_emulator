#ifndef _RUNNER_INCLUDED_
#define _RUNNER_INCLUDED_

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include "h8board.h"

class Runner {
    H8Board& h8;
    std::set<uint32_t> breakpoints;
    std::vector<uint32_t> call_stack;

    // todo: これらフラグ変数にロックが必要か確認する
    bool debug_mode;
    bool continue_mode;
    bool terminate;
    bool step_out_mode;
    bool print_pc_mode;

    bool load_file_to_memory(uint32_t address, char *filename);

    void print_help_command();
    void set_breakpoint_command(char *buf);
    void write_value_command(char *buf);

    void loop();

public:
    Runner(H8Board& h8);

    int proccess_debugger_command();
    void run(bool debug = false);

};

#endif
