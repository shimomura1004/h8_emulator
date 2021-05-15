#include "debugger.h"
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <semaphore.h>
#include "operation_map/operation_map.h"
#include "instructions/instruction_table.h"

// continue モード中は、Ctrl-c (mac は Ctrl-t) で連続実行を停止する
#define EXITING_MODE  (-1)  // 終了準備中状態
#define NORMAL_MODE   0     // 通常の実行状態
#define DEBUG_MODE    1     // デバッガによりコールスタックなどが監視されている状態
#define CONTINUE_MODE 2     // デバッグモードのサブモードで、連続で命令実行する状態
static volatile sig_atomic_t runner_mode = NORMAL_MODE;

static sem_t* sem;
static void sig_handler(int signo)
{
    switch (signo) {
#ifdef __APPLE__
    case SIGINFO:
#else
    case SIGINT:
#endif
        switch (runner_mode) {
        case NORMAL_MODE: // デバッグモードでなければすぐに終了
        case DEBUG_MODE:  // デバッグモード中にさらに Ctrl-C されたら終了
            // TODO: sigwait を使うほうが素直かも
            // すぐに終了させず、一旦 sem_post で通常のコンテキストに処理を戻す
            // その後、sem_wait しているスレッドから再度 SIGINT がくる
            runner_mode = EXITING_MODE;
            if (sem_post(sem) == -1) {
                write(2, "sem_post() failed\n", 18);
            }
            break;
        case CONTINUE_MODE:
            // デバッグモードで、一時的に連続実行している場合は止める
            runner_mode = DEBUG_MODE;
            if (sem_post(sem) == -1) {
                write(2, "sem_post() failed\n", 18);
                _exit(EXIT_FAILURE);
            }
            break;
        case EXITING_MODE:
            exit(1);
        default:
            break;
        }
        break;
    }
}

bool Debugger::load_file_to_memory(uint32_t address, char *filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return false;
    }

    int data;
    while ((data = fgetc(fp)) != EOF) {
        h8.mcu.write8(address++, data);
    }

    fclose(fp);
    return true;
}

void Debugger::set_breakpoint_command(uint32_t address)
{
    printf("Set breakpoint at 0x%08x\n", address);
    breakpoints.insert(address);
}

void Debugger::write_value_command(char *buf)
{
    uint32_t address = 0;
    uint32_t length = 0;
    uint32_t value;
    int ret = sscanf(buf + 1, "%x %u %x\n", &address, &length, &value);
    if (ret == 3) {
        switch (length) {
        case 1:
            fprintf(stderr, "Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write8(address, value);
            break;
        case 2:
            fprintf(stderr, "Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write16(address, value);
            break;
        case 4:
            fprintf(stderr, "Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write32(address, value);
            break;
        default:
            fprintf(stderr, "Syntax error in length.\n");
            break;
        }
    } else {
        fprintf(stderr, "Syntax error.\n");
    }
}

// TODO: メモリの内容を一部確認するコマンドがほしい
// TODO: レジスタを書き換えるコマンドがほしい
// TODO: SCI のレジスタを見るコマンドがほしい
// TODO: 様々な条件でのブレーク機能
//       特定のメモリアドレスへの書き込み時、レジスタが特定の値になったとき、など
#include "instructions/jsr.h"
#include "instructions/rts.h"
#include "instructions/rte.h"
#include "instructions/sleep.h"
// static bool step_out_mode = false;
static bool print_pc_mode = false;
int Debugger::proccess_debugger_command()
{
    if (runner_mode == CONTINUE_MODE) {
        if (breakpoints.find(h8.cpu.pc()) == breakpoints.end()) {
            return 0;
        } else {
            runner_mode = DEBUG_MODE;
        }

        // // TODO: バグがある
        // if (step_out_mode) {
        //     instruction_handler_t handler = OperationMap::lookup(&h8);
        //     if (handler == h8instructions::rts::rts)  {
        //         continue_mode = false;
        //         step_out_mode = false;
        //         return 0;
        //     }
        // }
    }

    h8.print_registers();

    char buf[256];
    while (1) {
        fprintf(stderr, "(h for help) > ");
        fflush(stdout);

        // 改行コードは残さない
        ssize_t size = ::read(0, buf, 255);
        buf[size - 1] = '\0';
        if (size == -1) {
            exit(1);
        }

        if (!this->debugger_parser.parse(buf)) {
            fprintf(stderr, "Unknown debugger command: %s\n", buf);
            continue;
        }

        switch (this->debugger_parser.get_command()) {
        case DebuggerParser::Command::HELP: {
            this->debugger_parser.print_help();
            break;
        }
        case DebuggerParser::Command::QUIT: {
            // デバッガ側から終了させる場合、フラグを立てて sem_wait しているスレッドを終了させる
            runner_mode = EXITING_MODE;
            if (sem_post(sem) == -1) {
                fprintf(stderr, "Error: sem_post() failed.\n");
            }
            return -1;
        }
        case DebuggerParser::Command::STEP: {
            // ステップ実行して sleep が実行されると Runner に処理が戻らなくなる
            // その状態で Ctrl-c されると即座に終了してしまうため
            // continue モードにすることでデバッガに戻れるようにする
            instruction_handler_t handler = operation_map::lookup(&h8);
            if (handler == h8instructions::sleep::sleep) {
                runner_mode = CONTINUE_MODE;
            }
            return 0;
        }
        case DebuggerParser::Command::CONTINUE: {
            runner_mode = CONTINUE_MODE;
            return 0;
        }
        case DebuggerParser::Command::STEP_OUT: {
            // // BUG: 動かない
            // runner_mode = CONTINUE_MODE;
            // step_out_mode = true;
            return 0;
        }
        case DebuggerParser::Command::BREAK_AT_ADDRESS: {
            uint32_t address = this->debugger_parser.get_address();
            set_breakpoint_command(address);
            break;
        }
        case DebuggerParser::Command::PRINT_REGISTERS: {
            h8.print_registers();
            break;
        }
        case DebuggerParser::Command::DUMP_MEMORY: {
            std::string parsed_filepath = this->debugger_parser.get_filepath();
            const char *filepath = parsed_filepath.empty() ? "core" : parsed_filepath.c_str();

            h8.mcu.dump(filepath);
            fprintf(stderr, "Memory dumped to '%s' file\n", filepath);
            break;
        }
        case DebuggerParser::Command::LOOKUP_INSTRUCTION: {
            instruction_handler_t handler = operation_map::lookup(&h8);
            fprintf(stderr, "%s\n", lookup_instruction_name(handler));
            break;
        }
        case DebuggerParser::Command::PRINT_INSTRUCTION: {
            instruction_parser_t parser = operation_map2::lookup(&h8);

            if (parser) {
                Instruction instruction;
                parser(&h8, instruction);
                instruction.print();
            } else {
                fprintf(stderr, "Error: unknown instruction\n");
            }
            break;
        }
        case DebuggerParser::Command::PRINT_CALL_STACK: {
            for (int i = call_stack.size() - 1; i >= 0; --i) {
                printf("%02d 0x%06x\n", i, call_stack[i]);
            }
            break;
        }
        case DebuggerParser::Command::WRITE_TO_REGISTER: {
            // TODO: 実装する
            write_value_command(buf);
            break;
        }
        case DebuggerParser::Command::TOGGLE_PRINT_PC: {
            print_pc_mode = !print_pc_mode;
            fprintf(stderr, "Print PC mode: %s\n", print_pc_mode ? "on" : "off");
            break;
        }
        default:
            break;
        }
    }
}

void Debugger::run(bool debug)
{
    sem = sem_open("h8emu_sem", O_CREAT, "0600", 1);

    pthread_t self_thread = pthread_self();
    // スリープ中の CPU を起こしにいくスレッド
    std::thread* sem_thread = new std::thread([&]{
        // シグナルハンドラで停止するとき sem_post で通常のコンテキストに処理が戻ってくる
        while (runner_mode != EXITING_MODE) {
            sem_wait(sem);
            // CPU がスリープ中でなければ単に無視される
            this->h8.wake_for_debugger();
        }
        // 通常のコンテキストで必要な処理を行ったあと
        // 再度 SIGINT を送ってシグナルハンドラに処理を戻す(終了させる)
#ifdef __APPLE__
        pthread_kill(self_thread, SIGINFO);
#else
        pthread_kill(self_thread, SIGINT);
#endif
    });

    runner_mode = debug ? DEBUG_MODE : NORMAL_MODE;
#ifdef __APPLE__
    signal(SIGINFO, sig_handler);
#else
    signal(SIGINT, sig_handler);
#endif

    int result = 0;

    while (1) {
        // bool interrupted = h8.handle_interrupt();
        h8.handle_interrupt();

        if (runner_mode == DEBUG_MODE || runner_mode == CONTINUE_MODE) {
            // TODO: なぜかスタックに積まれる数がとても多い
            // // 割込みが発生したら PC を保存
            // if (interrupted) {
            //     call_stack.push_back(h8.cpu.pc());
            // }

            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }

            // // TODO: instruction のパース結果を使ってもう少し情報を出力したい
            // instruction_handler_t handler = operation_map::lookup(&h8);
            // if ((handler == h8instructions::jsr::jsr_absolute_address) ||
            //     (handler == h8instructions::jsr::jsr_register_indirect))
            // {
            //     // 関数呼び出し時には今の PC を記録しておく
            //     call_stack.push_back(h8.cpu.pc());
            // }

            // if ((handler == h8instructions::rts::rts) ||
            //     (handler == h8instructions::rte::rte))
            // {
            //     // 関数・割込みからの復帰時はスタックから取り出し
            //     if (!call_stack.empty()) {
            //         call_stack.pop_back();
            //     } else {
            //         fprintf(stderr, "Warning: return when call stack is empty.\n");
            //     }
            // }
        }

        if (print_pc_mode) {
            printf("PC: 0x%08x\n", h8.cpu.pc());
        }

        // 次の命令を実行
        result = h8.step();

        if (result != 0) {
            fprintf(stderr, "Core dumped.\n");
            h8.mcu.dump("core");

            // クラッシュ時にデバッガに入る
            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }

            break;
        }
    }

    sem_close(sem);

    if (sem_thread->joinable()) {
        sem_thread->join();
    }

    delete sem_thread;
}
