#ifndef _DEBUGGER_PARSER_INCLUDED_
#define _DEBUGGER_PARSER_INCLUDED_

#include <string>

class DebuggerParser {
public:
    typedef enum {
        SYNTAX_ERROR,
        HELP,
        QUIT,

        // 命令実行
        STEP,
        CONTINUE,
        STEP_OUT,

        // ブレークポイント設定
        BREAK_AT_ADDRESS,

        // 状態確認
        PRINT_REGISTERS,
        DUMP_MEMORY,
        LOOKUP_INSTRUCTION,
        PRINT_INSTRUCTION,
        PRINT_CALL_STACK,

        // 状態更新
        WRITE_TO_REGISTER,

        // ユーティリティ
        TOGGLE_PRINT_PC,
    } Command;


protected:
    Command command;
    uint32_t address;
    std::string filepath;

    bool parse_help(std::string str);
    bool parse_quit(std::string str);
    bool parse_step(std::string str);
    bool parse_continue(std::string str);
    bool parse_step_out(std::string str);
    bool parse_break_at_adress(std::string str);
    bool parse_print_registers(std::string str);
    bool parse_dump_memory(std::string str);
    bool parse_lookup_instruction(std::string str);
    bool parse_print_instruction(std::string str);
    bool parse_print_call_stack(std::string str);
    bool parse_write_to_register(std::string str);
    bool parse_toggle_print_pc(std::string str);

public:
    bool parse(const char* buf);
    Command get_command();
    uint32_t get_address();
    std::string get_filepath();

    void print_help();

};

#endif
