#include "runner.h"
#include <signal.h>

// Ctrl-c or Ctrl-t でデバッグモードに入る
// コマンドラインからのシグナルはプロセスグループに対して発行されるので
// sender から明示的にシグナルを伝える必要はない
static volatile sig_atomic_t debug_mode = 0;
static volatile sig_atomic_t continue_mode = 0;
static void sig_handler(int signo)
{
    switch (signo) {
    case SIGINFO:
    case SIGINT:
        if (debug_mode && continue_mode) {
            continue_mode = false;
        } else if (!debug_mode) {
            debug_mode = true;
            continue_mode = false;
        } else if (debug_mode && !continue_mode) {
            exit(1);
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
        h8.mcu.write<8, uint8_t>(address++, data);
    }

    fclose(fp);
    return true;
}

void Runner::print_help_command()
{
    printf("  h: print help\n");
    printf("  r: print register status\n");
    printf("  d: dump memory\n");
    printf("  s: next step\n");
    printf("  c: continue execution until breakpoint\n");
    printf("  b (address): set break point\n");
    printf("  l (address) (filename): load file content to address\n");
    printf("  w (address) (length) (value): write value to memory\n");
    printf("  q: quit\n");
}

void Runner::set_breakpoint_command(char *buf)
{
    uint32_t address = 0;
    int ret = sscanf(buf + 1, "%x\n", &address);
    if (ret == 1) {
        printf("Set breakpoint at 0x%08x\n", address);
        breakpoints.insert(address);
    } else {
        printf("Syntax error\n");
    }
}

void Runner::load_file_command(char *buf)
{
    uint32_t address = 0;
    char filename[256];
    int ret = sscanf(buf + 1, "%x %s\n", &address, filename);
    if (ret == 2) {
        printf("Load %s to address 0x%08x\n", filename, address);
        bool ret = load_file_to_memory(address, filename);
        if (ret) {
            printf("%s was loaded successfully.\n", filename);
        } else {
            printf("%s not found. Ignored.\n", filename);
        }
    } else {
        fprintf(stderr, "Syntax error.\n");
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
            printf("Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write<8, uint8_t>(address, value);
            break;
        case 2:
            printf("Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write<16, uint16_t>(address, value);
            break;
        case 4:
            printf("Write 0x%x to [0x%06x]\n", value, address);
            h8.mcu.write<32, uint32_t>(address, value);
            break;
        default:
            fprintf(stderr, "Syntax error in length.\n");
            break;
        }
    } else {
        fprintf(stderr, "Syntax error.\n");
    }
}

// todo: メモリの内容を一部確認するコマンドがほしい
// todo: レジスタを書き換えるコマンドがほしい
// todo: SCI のレジスタを見るコマンドがほしい

// todo: continue mode と debug mode の違いは？
int Runner::proccess_debugger_command()
{
    if (continue_mode) {
        if (breakpoints.find(h8.pc) == breakpoints.end()) {
            return 0;
        } else {
            continue_mode = false;
        }
    }

    // SCI1(標準入出力につながっている)と標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    h8.print_registers();

    char buf[256];
    while (1) {
        printf("(h for help) > ");
        fflush(stdout);

        int i = 0;
        while (1) {
            // 標準入力はノンブロッキングで動作するため getchar はすぐに戻る
            int c = getchar();

            // EOF がきたら、単にデータがないということ
            if (c == EOF) {
                continue;
            }

            buf[i++] = c;

            // 改行コードがきたらコマンドを処理する
            if (c == '\n') {
                break;
            }
        }

        switch (buf[0]) {
        case 0x0a:
            return 0;
        case 'h':
            print_help_command();
            break;
        case 'r':
            h8.print_registers();
            break;
        case 'd':
            h8.mcu.dump("core");
            printf("Memory dumped to 'core' file\n");
            break;
        case 's':
            return 0;
        case 'c':
            continue_mode = true;
            return 0;
        case 'b':
            set_breakpoint_command(buf);
            break;
        case 'l':
            load_file_command(buf);
            break;
        case 'w':
            write_value_command(buf);
            break;
        case 'q':
            return -1;
        default:
            printf("Unknown debugger command: %c\n", buf[0]);
            break;
        }
    }
}

void Runner::run(bool debug)
{
    debug_mode = debug;
    signal(SIGINT, sig_handler);
    signal(SIGINFO, sig_handler);

    int result = 0;

    while (1) {
        if (debug_mode) {
            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }
        }

        // printf("PC: 0x%08x\n", h8.pc);

        result = h8.step();
        if (result != 0) {
            fprintf(stderr, "Core dumped.\n");
            h8.mcu.dump("core");
            break;
        }
    }

    h8.terminate = true;
}
