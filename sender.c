#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

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

void handle_send_command(int h8_serial_sock, char* buf)
{
    // ロードする OS は 8KB 以下を想定、それ以上のものを読むとスタックを壊す

    for (int i=0; i < strlen(buf); i++) {
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
    // todo: 転送開始が遅い
    // ロックしてる風だけど、しばらくすると動き出すのはなぜ？
    // serial_is_recv_enable 待ち？
    // メモリの排他制御のせい？
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
printf("222\n");
        // send body
        memset(os_buf, 0x1a, XMODEM_BLOCK_SIZE);
        ssize_t size = read(h8_serial_sock, os_buf, sizeof(char) * XMODEM_BLOCK_SIZE);
printf("111\n");
        // send checksum
        c = 0;
        for (int i = 0; i < XMODEM_BLOCK_SIZE; i++) {
            c += os_buf[i];
        }
        write(h8_serial_sock, &c, sizeof(char));
printf("aaa\n");
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
                int size = read(0, user_buf, LINE_BUFFER_SIZE - 1);
                if (size < 0) {
                    fprintf(stderr, "Error in reading user input.\n");
                    return 1;
                }
                user_buf[size] = '\0';

                if (strncmp(user_buf, SEND_COMMAND, sizeof(SEND_COMMAND) - 1) == 0) {
                    handle_send_command(h8_serial_sock, user_buf);
                } else {
                    // 特殊なコマンドでなければそのまま H8 に投げる
                    write(h8_serial_sock, user_buf, size);
                }
            }
        }
    }

    return 0;
}