#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <termios.h>

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1a

#define XMODEM_BLOCK_SIZE 128

#define LINE_BUFFER_SIZE 256

#define KEY_TO_COMMAND_MODE ':'
#define SEND_COMMAND ("send ")
#define EXEC_COMMAND ("exec ")

void handle_send_command(int h8_serial_sock, char* buf)
{
    // ロードする OS は 8KB 以下を想定、それ以上のものを読むとスタックを壊す

    for (unsigned int i=0; i < strlen(buf); i++) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
        }
    }
    printf("Sending [%s]\n", buf + sizeof(SEND_COMMAND) - 1);

    FILE *fp = fopen(buf + sizeof(SEND_COMMAND) - 1, "rb");
    if (!fp) {
        fprintf(stderr, "File not found: %s\n", buf + sizeof(SEND_COMMAND) - 1);
        return;
    }

    char c;
    unsigned char count = 1;
    unsigned char os_buf[XMODEM_BLOCK_SIZE];
    printf("Sending blocks");
    fflush(stdout);

    // 待ち時間は bootloader の wait のループ回数に依存する
    while (1) {
        putchar('.');
        fflush(stdout);

        // とりあえずエラー(NAK)でも無視する
        read(h8_serial_sock, &c, sizeof(char));

        // send header
        c = XMODEM_SOH;
        write(h8_serial_sock, &c, sizeof(char));

        // send count and inverted count
        c = count;
        write(h8_serial_sock, &c, sizeof(unsigned char));
        c = ~count;
        write(h8_serial_sock, &c, sizeof(unsigned char));

        // send body
        memset(os_buf, 0x1a, XMODEM_BLOCK_SIZE);
        ssize_t size = fread(os_buf, sizeof(char), XMODEM_BLOCK_SIZE, fp);
        write(h8_serial_sock, os_buf, sizeof(char) * XMODEM_BLOCK_SIZE);

        // send checksum
        c = 0;
        for (int i = 0; i < XMODEM_BLOCK_SIZE; i++) {
            c += os_buf[i];
        }
        write(h8_serial_sock, &c, sizeof(char));

        // 末尾までデータを送った場合
        if (size != XMODEM_BLOCK_SIZE) {
            // 最後に EOT を送り、ACK を待つ
            c = XMODEM_EOT;
            write(h8_serial_sock, &c, sizeof(char));
            read(h8_serial_sock, &c, sizeof(char));

            printf("done.\n");

            if (c != XMODEM_ACK) {
                printf("Error in xmodem protocol.\n");
            }
            break;
        }

        count++;
    }
}

void handle_exe_command(int h8_serial_sock, char* buf)
{
    char* command = buf + sizeof(EXEC_COMMAND) - 1;

    // プロセスを複製し、子プロセスとしてコマンドを実行
    int pid = fork();
    if (pid < 0) {
        perror("popen2");
        return;
    }

    if (pid == 0) {
        // 子プロセスの標準入出力をシリアルとつなげる
        dup2(h8_serial_sock, 0);
        dup2(h8_serial_sock, 1);
        execlp("sh", "sh", "-c", command, NULL);
        perror("CALL FAILED\n");
    }

    // 子プロセスが終了するまで待つ
    wait(NULL);
}

struct termios default_attribute;

void set_non_canonical()
{
    struct termios term;
    tcgetattr(fileno(stdin), &term);

    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(fileno(stdin), 0, &term);
}

void set_canonical()
{
    tcsetattr(fileno(stdin), 0, &default_attribute);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s (serial socket)\n", argv[0]);
        return 0;
    }

    int h8_serial_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (h8_serial_sock == -1)
    {
        fprintf(stderr, "Error: Failed to create socket.\n");
        return 1;
    }

    tcgetattr(fileno(stdin), &default_attribute);
    set_non_canonical();

    struct sockaddr_un sa = {0};
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, argv[1]);

    if (connect(h8_serial_sock, (struct sockaddr*) &sa, sizeof(struct sockaddr_un)) == -1)
    {
        fprintf(stderr, "Error: Failed to connect.\n");
        close(h8_serial_sock);
        return 1;
    }

    fd_set fdset;
    char h8_buf[LINE_BUFFER_SIZE];
    char user_buf[LINE_BUFFER_SIZE];

    while (1) {
        FD_ZERO(&fdset);
        FD_SET(0, &fdset);
        FD_SET(h8_serial_sock, &fdset);

        int ret = select(h8_serial_sock + 1, &fdset , NULL, NULL, NULL);
        if (ret != 0) {
            // h8 からの出力
            if (FD_ISSET(h8_serial_sock, &fdset)) {
                while (1) {
                    int size = read(h8_serial_sock, h8_buf, LINE_BUFFER_SIZE);
                    h8_buf[size++] = '\0';
                    printf("%s", h8_buf);
                    if (size != LINE_BUFFER_SIZE) {
                        break;
                    }
                }
                fflush(stdout);
            }
            // ユーザの入力を H8 に転送
            if (FD_ISSET(0, &fdset)) {
                // バックスペース入力は OS 側へ通知している、本来は OS 側で処理しないといけない

                // canonical 時は1行ごと、non-canonical 時は1文字ごとに読み取り
                int size = read(0, user_buf, LINE_BUFFER_SIZE - 1);
                if (size < 0) {
                    fprintf(stderr, "Error in reading user input.\n");
                    return 1;
                }
                user_buf[size] = '\0';

                // ":" が入力されたらコマンド入力モードに切り替え
                if (user_buf[0] == KEY_TO_COMMAND_MODE) {
                    set_canonical();
                    printf("(command) ");
                    fflush(stdout);
                    // todo: ここでフラグを立ててモードを区別できるようにする
                    continue;
                }

                // todo: コマンド入力モードでパースに失敗するとずっと canonical になる
                if (strncmp(user_buf, SEND_COMMAND, sizeof(SEND_COMMAND) - 1) == 0) {
                    // コマンド入力モードで入力されたコマンドの処理
                    handle_send_command(h8_serial_sock, user_buf);
                    // コマンド処理後は non-canonical モード(エコー・バッファリングなし)に戻る
                    set_non_canonical();
                } else if (strncmp(user_buf, EXEC_COMMAND, sizeof(EXEC_COMMAND) - 1) == 0) {
                    handle_exe_command(h8_serial_sock, user_buf);
                    set_non_canonical();
                } else {
                    // 特殊なコマンドでなければそのまま H8 に投げる
                    write(h8_serial_sock, user_buf, size);
                }
            }
        }
    }

    return 0;
}