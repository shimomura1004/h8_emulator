#include <thread>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "sci.h"
#include "sci_register.h"

void Sci::start(uint8_t index, InnerMemory& memory, bool& terminate, std::mutex& mutex)
{
    Sci sci(index, memory, terminate, mutex);
    sci.run();
}

Sci::Sci(uint8_t index, InnerMemory& memory, bool& terminate, std::mutex& mutex)
    : index(index)
    , terminate(terminate)
    , mutex(mutex)
    , sci_register(index, memory)
{
	timeout.tv_sec = 0; 
	timeout.tv_usec = 0;
}

Sci::~Sci()
{
    close(ser_fd);
}

// CPU から送信要求がきたか？
bool Sci::send_requested() {
    // SSR_TDRE が 0 になったら、要求がきたということ
    return sci_register.get_ssr_tdre() == false;
}

void Sci::process_recv_request()
{
    // デバッガと標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    // select が fdset をクリアするので毎回設定する必要がある
    FD_ZERO(&fdset);
	FD_SET(0, &fdset);
    FD_SET(ser_fd, &fdset);

	int ret = select(ser_fd + 1, &fdset , NULL , NULL , &timeout);
    if (ret == 1) {
        // 標準入力にデータがある
        if (FD_ISSET(0, &fdset)) {
            while (1) {
                buffer.push(fgetc(stdin));
                if (buffer.back() == 0x0a) {
                    break;
                }
            }
        }

        // 名前付きパイプにデータがある
        if (FD_ISSET(ser_fd, &fdset)) {
            ssize_t size;
            char c;
            while (1) {
                size = read(ser_fd, &c, 1);
                buffer.push(c);
                if (size == 0) {
                    break;
                }
            }
        }
    }

    // まだ H8 が処理していないので何もしない
    if (sci_register.get_ssr_rdrf()) {
        return;
    }

    // バッファにデータがあるのであれば H8 に送信
    if (!buffer.empty()) {
        // H8 に渡すデータは RDR に書き込んでおく
        sci_register.set_rdr(buffer.front());
        buffer.pop();

        // RDRF を 1 にして H8 に通知
        sci_register.set_ssr_rdrf(true);
    }
}

void Sci::process_send_request() {
    // デバッガと標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    if (send_requested()) {
        // データは TDR に入っている
        uint8_t data = sci_register.get_tdr();

        // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
        putc(data, stdout);
        fflush(stdout);

        // 送信が終わったらSSR_TDRE を 1 にして送信可能を通知
        sci_register.set_ssr_tdre(true);
    }
}

// 途中で標準入力からバイナリを流し込みたいので
// sci は名前付きパイプを作ってそこからも読むようにする
void Sci::run()
{
    char pipe_name[5];
    sprintf(pipe_name, "ser%d", index);
    int ret = mkfifo("pipe_name", 0666);
    if (ret == -1) {
        fprintf(stderr, "Warning: Failed to create ser1 (maybe already exits?)\n");
    } else {
        printf("Created named pipe: %s\n", pipe_name);
    }
    ser_fd = open("ser1", O_RDONLY | O_NONBLOCK);
    if (ser_fd == -1) {
        fprintf(stderr, "Error: Failed to open named pipe %s\n", pipe_name);
    }

    printf("SCI(%d) started\n", index);

    while (!terminate) {
        // todo: イベントドリブンにする

        // 少し動作を遅くする
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // 送信要求がきていたら処理
        process_send_request();

        // (H8 の SCI1 がつながっている)標準入力からデータがきたら処理する
        process_recv_request();
    }
}