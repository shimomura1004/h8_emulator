#include "runner.h"

void Runner::print_help()
{
    printf("  h: print help\n");
    printf("  r: print register status\n");
    printf("  d: dump memory\n");
    printf("  s: next step\n");
    printf("  c: continue execution until breakpoint\n");
    printf("  b (address): set break point\n");
    printf("  q: quit\n");
}

void Runner::set_breakpoint(char *buf)
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

int Runner::proccess_debugger_command()
{
    if (continue_mode) {
        if (breakpoints.find(h8.pc) == breakpoints.end()) {
            return 0;
        } else {
            continue_mode = false;
        }
    }

    h8.print_registers();

    char buf[256];
    while (1) {
        printf("(h for help) > ");
        if (fgets(buf, 256, stdin) == NULL) {
            return -1;
        }
        switch (buf[0]) {
        case 0x0a:
            return 0;
        case 'h':
            print_help();
            break;
        case 'r':
            h8.print_registers();
            break;
        case 'd':
            h8.memory.dump("core");
            printf("Memory dumped to 'core' file\n");
            break;
        case 's':
            return 0;
        case 'c':
            continue_mode = true;
            return 0;
        case 'b':
            set_breakpoint(buf);
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
    int result = 0;

    while (1) {
        if (debug) {
            int r = proccess_debugger_command();
            if (r != 0) {
                break;
            }
        }

        result = h8.step();
        if (result != 0) {
            fprintf(stderr, "Core dumped.\n");
            h8.memory.dump("core");
            break;
        }
    }

    h8.terminate = true;
}
