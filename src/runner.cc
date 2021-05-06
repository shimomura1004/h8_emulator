#include "runner.h"
#include <signal.h>
#include <string.h>
#include <thread>
#include "operation_map/operation_map.h"
#include "instructions/instruction_table.h"

#include "instructions/cmp.h"

// Runner のモード
//   debug モードはデバッガによりコールスタックなどが監視されている状態
//   continue モードは debug モードのサブモードで、ステップ実行せず連続で命令実行する

// bug: Ctrl-t してもすぐに止まらない(sender でなにか入力すると止まる？)
//      割込みが発生するまでどこかで止まっている？
//      -> sleep で返ってこなくなってた
// Ctrl-c or Ctrl-t でデバッグモードに入る

bool Runner::load_file_to_memory(uint32_t address, char *filename)
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

void Runner::print_help_command()
{
    fprintf(stderr, "  help: print help\n");
    fprintf(stderr, "  r: print register status\n");
    fprintf(stderr, "  dump: dump memory\n");
    fprintf(stderr, "  s: next step\n");
    fprintf(stderr, "  continue: continue execution until breakpoint\n");
    fprintf(stderr, "  b (address): set break point\n");
    fprintf(stderr, "  lookup: display next instruction\n");
    fprintf(stderr, "  stepout: step-out from current function\n");
    fprintf(stderr, "  printpc: toggle printing pc\n");
    fprintf(stderr, "  writereg (address) (length) (value): write value to memory\n");
    fprintf(stderr, "  quit: quit\n");
}

void Runner::set_breakpoint_command(char *buf)
{
    uint32_t address = 0;
    int ret = sscanf(buf + 1, "%x\n", &address);
    if (ret == 1) {
        fprintf(stderr, "Set breakpoint at 0x%08x\n", address);
        breakpoints.insert(address);
    } else {
        fprintf(stderr, "Syntax error\n");
    }
}

void Runner::write_value_command(char *buf)
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

// todo: 先頭の文字がぶつかると意図しないコマンドが実行されてしまう
// todo: 別ファイルへ
#define MATCH(buf, command) (strncmp(buf, command, sizeof(command) - 1) == 0)
#define HELP1       "h"
#define HELP2       "help"
#define REG1        "r"
#define REG2        "reg"
#define DUMP        "dump"
#define STEP1       "s"
#define STEP2       "step"
#define CONTINUE1   "c"
#define CONTINUE2   "continue"
#define BREAK1      "b "
#define BREAK2      "break"
#define LOOKUP      "lookup"
#define STEP_OUT    "so"
#define PRINT_PC_MODE   "printpc"
#define WRITE_REG   "writereg"
#define CALL_STACK  "bt"
#define QUIT1       "q"
#define QUIT2       "quit"
#define PRINT       "p"

// todo: メモリの内容を一部確認するコマンドがほしい
// todo: レジスタを書き換えるコマンドがほしい
// todo: SCI のレジスタを見るコマンドがほしい
// todo: 様々な条件でのブレーク機能
//       特定のメモリアドレスへの書き込み時、レジスタが特定の値になったとき、など
#include "instructions/jsr.h"
#include "instructions/rts.h"
#include "instructions/rte.h"

void Runner::loop()
{
    int result = 0;

    while (!terminate) {
        // bool interrupted = h8.handle_interrupt();
        h8.handle_interrupt();

        if (this->debug_mode) {
            // // 割込みが発生したら PC を保存
            // if (interrupted) {
            //     printf("PUSHED 0x%x\n", h8.cpu.pc());
            //     call_stack.push_back(h8.cpu.pc());
            // }

            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }

            // // todo: instruction のパース結果を使ってもう少し情報を出力したい
            // instruction_handler_t handler = operation_map::lookup(&h8);
            // if ((handler == h8instructions::jsr::jsr_absolute_address) ||
            //     (handler == h8instructions::jsr::jsr_register_indirect))
            // {
            //     // 関数呼出し時に PC を記録しておく
            //     printf("PUSHED 0x%x\n", h8.cpu.pc());
            //     call_stack.push_back(h8.cpu.pc());
            // }

            // if ((handler == h8instructions::rts::rts) ||
            //     (handler == h8instructions::rte::rte))
            // {
            //     // 関数・割込みからの復帰時はスタックから取り出し
            //     if (!call_stack.empty()) {
            //         printf("POPED 0x%x\n", call_stack.back());
            //         call_stack.pop_back();
            //     } else {
            //         fprintf(stderr, "Warning: return when call stack is empty.\n");
            //     }
            // }
        }

        if (this->print_pc_mode) {
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
}

Runner::Runner(H8Board& h8)
    : h8(h8)
    , debug_mode(false)
    , continue_mode(false)
    , terminate(false)
    , step_out_mode(false)
    , print_pc_mode(false)
{}

int Runner::proccess_debugger_command()
{
    if (this->continue_mode) {
        if (breakpoints.find(h8.cpu.pc()) == breakpoints.end()) {
            return 0;
        } else {
            this->continue_mode = false;
        }

        // // todo: バグがある
        // if (this->step_out_mode) {
        //     instruction_handler_t handler = OperationMap::lookup(&h8);
        //     if (handler == h8instructions::rts::rts)  {
        //         this->continue_mode = false;
        //         this->step_out_mode = false;
        //         return 0;
        //     }
        // }
    }

    h8.print_registers();

    char buf[256];
    while (1) {
        fprintf(stderr, "(h for help) > ");
        fflush(stdout);

        int i = 0;
        while (!terminate) {
            // bug: ブロッキングで動作しているため、終了を指示してもすぐに終了しない
            int c = getchar();

            // EOF がきたら、単にデータがないということ
            if (c == EOF) {
                continue;
            }

            buf[i++] = c;

            // 改行コードがきたらコマンドを処理する
            if (c == '\n') {
                buf[i - 1] = '\0';
                break;
            }
        }

        if (terminate) {
            return 0;
        }

        if (buf[0] == '\0') {
            return 0;
        } else if (MATCH(buf, HELP1) || MATCH(buf, HELP2)) {
            print_help_command();
            continue;
        } else if (MATCH(buf, REG1) || MATCH(buf, REG2)) {
            h8.print_registers();
            continue;
        } else if (MATCH(buf, DUMP)) {
            h8.mcu.dump("core");
            fprintf(stderr, "Memory dumped to 'core' file\n");
            continue;
        } else if (MATCH(buf, STEP1) || MATCH(buf, STEP2)) {
            return 0;
        } else if (MATCH(buf, CONTINUE1) || MATCH(buf, CONTINUE2)) {
            this->continue_mode = true;
            return 0;
        } else if (MATCH(buf, BREAK1) || MATCH(buf, BREAK2)) {
            set_breakpoint_command(buf);
            continue;
        } else if (MATCH(buf, LOOKUP)) {
            instruction_handler_t handler = operation_map::lookup(&h8);
            fprintf(stderr, "%s\n", lookup_instruction_name(handler));
            continue;
        } else if (MATCH(buf, STEP_OUT)) {
            this->continue_mode = true;
            this->step_out_mode = true;
            continue;
        } else if (MATCH(buf, PRINT_PC_MODE)) {
            this->print_pc_mode = !this->print_pc_mode;
            fprintf(stderr, "Print PC mode: %s\n", this->print_pc_mode ? "on" : "off");
            continue;
        } else if (MATCH(buf, WRITE_REG)) {
            write_value_command(buf);
            continue;
        } else if (MATCH(buf, CALL_STACK)) {
            for (int i = call_stack.size() - 1; i >= 0; --i) {
                printf("%02d 0x%06x\n", i, call_stack[i]);
            }
            continue;
        } else if (MATCH(buf, PRINT)) {
            instruction_parser_t parser = operation_map2::lookup(&h8);

            if (parser) {
                Instruction instruction;
                parser(&h8, &instruction);
                
                char name[8];
                char op1[32];
                char op2[32];
                instruction.stringify_name(name);
                instruction.op1.stringify(op1);
                instruction.op2.stringify(op2);

                printf("%s %s,%s\n", name, op1, op2);
            } else {
                fprintf(stderr, "Error: unknown instruction\n");
            }
            
            continue;
        } else if (MATCH(buf, QUIT1) || MATCH(buf, QUIT2)) {
            return -1;
        } else {
            fprintf(stderr, "Unknown debugger command: %s\n", buf);
            continue;
        }

        return 0;
    }
}

void Runner::run(bool debug)
{
    this->debug_mode = debug;

    // メインの処理を行うスレッドを起動
    std::thread* loop = new std::thread(&Runner::loop, this);

    // 以降、このスレッドはシグナル処理だけを行う
    int sig;
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
#ifdef __APPLE__
    sigaddset(&block_mask, SIGINFO);
#endif
    sigprocmask(SIG_SETMASK, &block_mask, NULL);

    while (!terminate) {
        if (sigwait(&block_mask, &sig) == 0) {
            switch (sig) {
            case SIGINT:
                if (!this->debug_mode) {
                    // デバッグモードでなければすぐに終了
                    terminate = true;
                    h8.wake_for_debugger();
                } else if (this->continue_mode) {
                    // デバッグモードで、一時的に連続実行している場合は止める
                    this->continue_mode = false;
                    h8.wake_for_debugger();
                } else {
                    // デバッグモード中で、停止中にさらに Ctrl-C されたら終了
                    terminate = true;
                    h8.wake_for_debugger();
                }
                break;
#ifdef __APPLE__
            case SIGINFO:
                if (!this->debug_mode) {
                    terminate = true;
                    h8.wake_for_debugger();
                } else if (this->continue_mode) {
                    this->continue_mode = false;
                    h8.wake_for_debugger();
                }
                break;
#endif
            default:
                printf("Unhandled signal (%d)\n", sig);
                break;
            }
        }
    }

    // スレッドを止めてから終了
    if (loop->joinable()) {
        loop->join();
    }
    delete loop;
}
