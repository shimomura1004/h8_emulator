#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#define R (0)
#define W (1)

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1a

#define XMODEM_BLOCK_SIZE 128

#define LINE_BUFFER_SIZE 256

#define SEND_COMMAND (":send ")
#define QUIT_COMMAND (":quit")

static int fd_r, fd_w;

// todo: h8300h への signal の伝達
// 単純に SIGINT を転送するとクラッシュする

int popen2(int argc, char* argv[], int* fd_r, int* fd_w)
{
    int pipe_c2p[2],pipe_p2c[2];
    int pid;

    if (pipe(pipe_c2p) < 0) {
        perror("popen2");
        return(-1);
    }
    if (pipe(pipe_p2c) < 0) {
        perror("popen2");
        close(pipe_c2p[R]);
        close(pipe_c2p[W]);
        return -1;
    }

    if ((pid=fork()) < 0){
        perror("popen2");
        close(pipe_c2p[R]);
        close(pipe_c2p[W]);
        close(pipe_p2c[R]);
        close(pipe_p2c[W]);
        return -1;
    }
    if (pid == 0){
        // 子プロセス
        close(pipe_p2c[W]);
        close(pipe_c2p[R]);
        dup2(pipe_p2c[R], 0);
        dup2(pipe_c2p[W], 1);
        close(pipe_p2c[R]);
        close(pipe_c2p[W]);

        // h8300h を子プロセスとして起動
        // argv[argc] は C の仕様上 null なので、execv に直接渡していい
        argv[0] = "./h8300h";
        if (execvp(argv[0], argv) < 0) {
            perror("popen2");
            close(pipe_p2c[R]);
            close(pipe_c2p[W]);
            exit(1);
        }
    }

    close(pipe_p2c[R]);
    close(pipe_c2p[W]);
    *fd_w=pipe_p2c[W];
    *fd_r=pipe_c2p[R];

    return(pid);
}

int handle_send_command(char* buf)
{
    // ロードする OS は 8KB 以下を想定、それ以上のものを読むとスタックを壊す

    for (int i=0; i < strlen(buf); i++) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
        }
    }
    printf("Sending [%s].\n", buf + sizeof(SEND_COMMAND) - 1);

    FILE *fp = fopen(buf + sizeof(SEND_COMMAND) - 1, "rb");
    if (!fp) {
        fprintf(stderr, "File not found: %s.\n", buf + sizeof(SEND_COMMAND) - 1);
        return -1;
    }

    int response;
    char c;
    unsigned char count = 1;
    unsigned char os_buf[XMODEM_BLOCK_SIZE];
    printf("Sending blocks");
    fflush(stdout);
    while (1) {
        putchar('.');
        fflush(stdout);

        // とりあえずエラー(NAK)でも無視する
        read(fd_r, &response, 1);

        // send header
        c = XMODEM_SOH;
        write(fd_w, &c, 1);

        // send inverted header
        write(fd_w, &count, 1);
        char invert_count = ~count;
        write(fd_w, &invert_count, 1);

        // send body
        memset(os_buf, EOF, XMODEM_BLOCK_SIZE);
        int size = fread(os_buf, sizeof(char), XMODEM_BLOCK_SIZE, fp);
        write(fd_w, os_buf, XMODEM_BLOCK_SIZE);

        // send checksum
        unsigned char check_sum = 0;
        for (int i = 0; i < XMODEM_BLOCK_SIZE; i++) {
            check_sum += os_buf[i];
        }
        write(fd_w, &check_sum, 1);

        // 末尾までデータを送った場合
        if (size != XMODEM_BLOCK_SIZE) {
            // 最後に EOT を送り、ACK を待つ
            c = XMODEM_EOT;
            write(fd_w, &c, 1);
            read(fd_r, &response, 1);

            printf("done.\n");

            if (response != XMODEM_ACK) {
                printf("Error in xmodem protocol.\n");
            }
            break;
        }

        count++;
    }

    fclose(fp);

    return 1;
}

static void sig_handler(int signo)
{
    switch (signo) {
    case SIGCHLD:
        exit(1);
        break;
    case SIGINT:
        break;
    case SIGINFO:
        break;
    }
}

int main(int argc, char* argv[])
{
    signal(SIGCHLD, sig_handler);
    signal(SIGINT, sig_handler);

    char h8_buf[LINE_BUFFER_SIZE];
    char user_buf[LINE_BUFFER_SIZE];

    fd_set fdset;

    int h8300h_pid = popen2(argc, argv, &fd_r, &fd_w);

    while (1) {
        FD_ZERO(&fdset);
        FD_SET(0, &fdset);
        FD_SET(fd_r, &fdset);

        int ret = select(fd_r + 1, &fdset , NULL, NULL, NULL);
        if (ret != 0) {
            // h8 からの出力
            if (FD_ISSET(fd_r, &fdset)) {
                int size = 0;
                while (1) {
                    size = read(fd_r, h8_buf, LINE_BUFFER_SIZE - 1);
                    h8_buf[size] = '\0';
                    printf("%s", h8_buf);
                    if (size != LINE_BUFFER_SIZE - 1) {
                        break;
                    }
                }
                fflush(stdout);
            }
            // ユーザの入力
            if (FD_ISSET(0, &fdset)) {
                int size = read(0, user_buf, LINE_BUFFER_SIZE - 1);
                if (size < 0) {
                    fprintf(stderr, "Error in reading user input.\n");
                    return -1;
                }
                user_buf[size] = '\0';

                if (strncmp(user_buf, SEND_COMMAND, sizeof(SEND_COMMAND) - 1) == 0) {
                    handle_send_command(user_buf);
                } else if (strncmp(user_buf, QUIT_COMMAND, sizeof(QUIT_COMMAND) - 1) == 0) {
                    kill(h8300h_pid, SIGKILL);
                    break;
                } else {
                    // 特殊なコマンドでなければそのまま H8 に投げる
                    write(fd_w, user_buf, size);
                }
            }
        }
    }

    return 0;
}