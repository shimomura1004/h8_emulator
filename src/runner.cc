#include "runner.h"
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <thread>
#include <semaphore.h>
#include "operation_map/operation_map.h"
#include "instructions/instruction_table.h"

#include "instructions/cmp.h"

// todo: ステップ実行して sleep に入ったら、モードを切り替える
// デバッグモード中の判定となり、再度 Ctrl-C を押すと終了してしまうため

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
            // todo: sigwait を使うほうが素直かも
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
static bool step_out_mode = false;
static bool print_pc_mode = false;
int Runner::proccess_debugger_command()
{
    if (runner_mode == CONTINUE_MODE) {
        if (breakpoints.find(h8.cpu.pc()) == breakpoints.end()) {
            return 0;
        } else {
            runner_mode = DEBUG_MODE;
        }

        // // todo: バグがある
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

        if (buf[0] == '\0') {
            return 0;
        } else if (MATCH(buf, HELP1) || MATCH(buf, HELP2)) {
            print_help_command();
        } else if (MATCH(buf, REG1) || MATCH(buf, REG2)) {
            h8.print_registers();
        } else if (MATCH(buf, DUMP)) {
            h8.mcu.dump("core");
            fprintf(stderr, "Memory dumped to 'core' file\n");
        } else if (MATCH(buf, STEP1) || MATCH(buf, STEP2)) {
            return 0;
        } else if (MATCH(buf, CONTINUE1) || MATCH(buf, CONTINUE2)) {
            runner_mode = CONTINUE_MODE;
            return 0;
        } else if (MATCH(buf, BREAK1) || MATCH(buf, BREAK2)) {
            set_breakpoint_command(buf);
        } else if (MATCH(buf, LOOKUP)) {
            instruction_handler_t handler = operation_map::lookup(&h8);
            fprintf(stderr, "%s\n", lookup_instruction_name(handler));
        } else if (MATCH(buf, STEP_OUT)) {
            runner_mode = CONTINUE_MODE;
            step_out_mode = true;
        } else if (MATCH(buf, PRINT_PC_MODE)) {
            print_pc_mode = !print_pc_mode;
            fprintf(stderr, "Print PC mode: %s\n", print_pc_mode ? "on" : "off");
        } else if (MATCH(buf, WRITE_REG)) {
            write_value_command(buf);
        } else if (MATCH(buf, CALL_STACK)) {
            for (int i = call_stack.size() - 1; i >= 0; --i) {
                printf("%02d 0x%06x\n", i, call_stack[i]);
            }
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
        } else if (MATCH(buf, QUIT1) || MATCH(buf, QUIT2)) {
            // デバッガ側から終了させる場合、フラグを立てて sem_wait しているスレッドを終了させる
            runner_mode = EXITING_MODE;
            if (sem_post(sem) == -1) {
                fprintf(stderr, "Error: sem_post() failed.\n");
            }
            return -1;
        } else {
            fprintf(stderr, "Unknown debugger command: %s\n", buf);
        }
    }
}

void Runner::run(bool debug)
{
    sem = sem_open("h8emu_sem", O_CREAT, "0600", 1);

    pthread_t self_thread = pthread_self();
    std::thread* sem_thread = new std::thread([&]{
        // シグナルハンドラで停止するとき、 sem_post で通常のコンテキストに処理が戻ってくる
        while (runner_mode != EXITING_MODE) {
            sem_wait(sem);
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
        bool interrupted = h8.handle_interrupt();

        if (runner_mode == DEBUG_MODE || runner_mode == CONTINUE_MODE) {
            // 割込みが発生したら PC を保存
            if (interrupted) {
                // call_stack.push_back(h8.pc);
            }

            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }

            // todo: instruction のパース結果を使ってもう少し情報を出力したい
            // instruction_handler_t handler = operation_map::lookup(&h8);
            // if ((handler == h8instructions::jsr::jsr_absolute_address) ||
            //     (handler == h8instructions::jsr::jsr_register_indirect))
            // {
            //     // 関数呼び出し時には今の PC を記録しておく
            //     call_stack.push_back(h8.pc);
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
