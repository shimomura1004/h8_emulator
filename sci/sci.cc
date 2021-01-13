#include <thread>
#include <chrono>
#include <fcntl.h>

#include "sci.h"
#include "sci_register.h"

// todo: sci ごとに名前付きパイプを作って、そこに入出力をつなげるほうがよさそう

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
    int flags;
    flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
}

// CPU から送信要求がきたか？
bool Sci::send_requested() {
    // SSR_TDRE が 0 になったら、要求がきたということ
    return sci_register.get_ssr_tdre() == false;
}

// ユーザの入力を H8 に送信
void Sci::process_recv_request()
{
    // デバッガと標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    int c;
    while (1) {
        c = getchar();

        if (c == EOF) {
            break;
        }

        buffer.push(c);
    }

    // まだ H8 が処理していないので何もしない
    if (sci_register.get_ssr_rdrf()) {
        return;
    }

    // バッファにデータがあるのであれば H8 に送信
    if (!buffer.empty()) {
        // H8 に渡すデータは RDR に書き込んでおく
        sci_register.set_rdr(buffer.front());

        // RDRF を 1 にして H8 に通知
        sci_register.set_ssr_rdrf(true);

        buffer.pop();
    }
}

// H8 からの出力をユーザ(標準出力)に表示
void Sci::process_send_request() {
    // デバッガと標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    if (send_requested()) {
        // データは TDR に入っている
        uint8_t data = sci_register.get_tdr();

        // 送信が終わったらSSR_TDRE を 1 にして送信可能を通知
        sci_register.set_ssr_tdre(true);

        // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
        putc(data, stdout);
        fflush(stdout);
    }
}

void Sci::run()
{
    printf("SCI(%d) started\n", index);

    while (!terminate) {
        // todo: イベントドリブンにする

        // 少し動作を遅くする
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // 送信要求がきていたら処理
        process_send_request();

        // (H8 の SCI1 がつながっている)標準入力からデータがきたら処理する
        process_recv_request();
    }
}