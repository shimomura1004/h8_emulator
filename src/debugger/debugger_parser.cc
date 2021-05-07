#include "debugger_parser.h"
#include <regex>

bool DebuggerParser::parse_help(std::string str)
{
    static std::regex re1("^help$");
    static std::regex re2("^[h\\?]$"); // 'h' or '?'
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::HELP;
    }
    return ret;
}

bool DebuggerParser::parse_quit(std::string str)
{
    static std::regex re1("^quit$");
    static std::regex re2("^q$");
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::QUIT;
    }
    return ret;
}

bool DebuggerParser::parse_step(std::string str)
{
    static std::regex re1("^step$");
    static std::regex re2("^s$");
    static std::regex re3("^$");
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2)
            || std::regex_match(str, re3);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::STEP;
    }
    return ret;
}

bool DebuggerParser::parse_continue(std::string str)
{
    static std::regex re1("^continue$");
    static std::regex re2("^c$");
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::CONTINUE;
    }
    return ret;
}

bool DebuggerParser::parse_step_out(std::string str)
{
    static std::regex re1("^so$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::STEP_OUT;
    }
    return ret;
}

bool DebuggerParser::parse_break_at_adress(std::string str)
{
    static std::regex re1("^break +(0[xX][0-9a-fA-F]+)$");
    static std::regex re2("^b +(0[xX][0-9a-fA-F]+)$");
    std::smatch match;
    bool ret = std::regex_match(str, match, re1)
            || std::regex_match(str, match, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::BREAK_AT_ADDRESS;
        this->parse_result.address = std::stoul(match[1].str(), nullptr, 16);
    }
    return ret;
}

bool DebuggerParser::parse_print_registers(std::string str)
{
    static std::regex re1("^reg$");
    static std::regex re2("^r$");
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::PRINT_REGISTERS;
    }
    return ret;
}

bool DebuggerParser::parse_dump_memory(std::string str)
{
    static std::regex re1("^dump$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::DUMP_MEMORY;
    }
    return ret;
}

bool DebuggerParser::parse_lookup_instruction(std::string str)
{
    static std::regex re1("^lookup$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::LOOKUP_INSTRUCTION;
    }
    return ret;
}
bool DebuggerParser::parse_print_instruction(std::string str)
{
    static std::regex re1("^p$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::PRINT_INSTRUCTION;
    }
    return ret;
}

bool DebuggerParser::parse_print_call_stack(std::string str)
{
    static std::regex re1("^backtrace$");
    static std::regex re2("^bt$");
    bool ret = std::regex_match(str, re1)
            || std::regex_match(str, re2);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::PRINT_CALL_STACK;
    }
    return ret;
}

bool DebuggerParser::parse_write_to_register(std::string str)
{
    static std::regex re1("^writereg$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::WRITE_TO_REGISTER;
    }
    return ret;
}

bool DebuggerParser::parse_toggle_print_pc(std::string str)
{
    static std::regex re1("^printpc$");
    bool ret = std::regex_match(str, re1);
    if (ret) {
        this->parse_result.command = DebuggerParser::Command::TOGGLE_PRINT_PC;
    }
    return ret;
}

bool DebuggerParser::parse(const char* buf)
{
    std::string str(buf);

    return parse_help(str)
        || parse_quit(str)
        || parse_step(str)
        || parse_continue(str)
        || parse_step_out(str)
        || parse_break_at_adress(str)
        || parse_print_registers(str)
        || parse_dump_memory(str)
        || parse_lookup_instruction(str)
        || parse_print_instruction(str)
        || parse_print_call_stack(str)
        || parse_write_to_register(str)
        || parse_toggle_print_pc(str);
}

DebuggerParser::Command DebuggerParser::get_command()
{
    return this->parse_result.command;
}

uint32_t DebuggerParser::get_address()
{
    if (this->parse_result.command != DebuggerParser::Command::BREAK_AT_ADDRESS) {
        fprintf(stderr, "Error: invalid access to parse result: address\n");
        return 0;
    }

    return this->parse_result.address;
}
