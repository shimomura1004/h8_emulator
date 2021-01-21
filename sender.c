#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

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

void handle_send_command(FILE* rx, char* buf)
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
        fgetc(rx);

        // send header
        fputc(XMODEM_SOH, rx);

        // send inverted header
        fputc(count, rx);
        fputc(~count, rx);

        // send body
        memset(os_buf, EOF, XMODEM_BLOCK_SIZE);
        int size = fread(os_buf, sizeof(char), XMODEM_BLOCK_SIZE, fp);
        fwrite(os_buf, sizeof(char), XMODEM_BLOCK_SIZE, rx);

        // send checksum
        unsigned char check_sum = 0;
        for (int i = 0; i < XMODEM_BLOCK_SIZE; i++) {
            check_sum += os_buf[i];
        }
        fputc(check_sum, rx);

        // 末尾までデータを送った場合
        if (size != XMODEM_BLOCK_SIZE) {
            // 最後に EOT を送り、ACK を待つ
            fputc(XMODEM_EOT, rx);
            int response = fgetc(rx);

            printf("done.\n");

            if (response != XMODEM_ACK) {
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
        fprintf(stderr, "Usage: %s (ser num)\n", argv[0]);
        return 0;
    }

    uint8_t index;
    int ret = sscanf(argv[1], "%hhu", &index);
    if (ret != 1) {
        fprintf(stderr, "Usage: %s (ser num)\n", argv[0]);
        return 1;
    }

    char tx_pipe_name[16];
    char rx_pipe_name[16];

    sprintf(tx_pipe_name, "tx_%d", index);
    sprintf(rx_pipe_name, "rx_%d", index);

    // H8 の RX にデータを送信する、TX からデータを読み取る
    FILE* tx = fopen(tx_pipe_name, "r");
    FILE* rx = fopen(rx_pipe_name, "w");

    if (!tx || !rx) {
        fprintf(stderr, "Failed to open serial port\n");
        return 1;
    }

    fd_set fdset;
    const int tx_fd = fileno(tx);
    char user_buf[LINE_BUFFER_SIZE];

    while (1) {
        FD_ZERO(&fdset);
        FD_SET(0, &fdset);
        FD_SET(tx_fd, &fdset);

        int ret = select(tx_fd + 1, &fdset , NULL, NULL, NULL);
        if (ret != 0) {
            // h8 からの出力
            if (FD_ISSET(tx_fd, &fdset)) {
                printf("from h8\n");
                int c;
                while ((c = fgetc(tx)) != EOF) {
                    putchar(c);
                }
                fflush(stdout);
            }
            // ユーザの入力を H8 に転送
            if (FD_ISSET(0, &fdset)) {
                printf("user input!\n");
                int size = read(0, user_buf, LINE_BUFFER_SIZE - 1);
                if (size < 0) {
                    fprintf(stderr, "Error in reading user input.\n");
                    return 1;
                }
                user_buf[size] = '\0';

                if (strncmp(user_buf, SEND_COMMAND, sizeof(SEND_COMMAND) - 1) == 0) {
                    handle_send_command(rx, user_buf);
                } else {
                    // 特殊なコマンドでなければそのまま H8 に投げる
                    // write(rx, user_buf, size);
                    printf("sending '%s'\n", user_buf);
                    fprintf(rx, "%s", user_buf);
                    fflush(rx);
                }
            }
        }
    }

    return 0;
}