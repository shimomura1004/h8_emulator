#include <thread>
#include <chrono>

#include "sci.h"
#include "sci_register.h"

void Sci::start(uint8_t index, InnerMemory& memory, bool& terminate)
{
    Sci sci(index, memory, terminate);
    sci.run();
}

Sci::Sci(uint8_t index, InnerMemory& memory, bool& terminate)
    : index(index), terminate(terminate), sci_register(index, memory)
{
}

// CPU から送信要求がきたか？
bool Sci::send_requested() {
    // SSR_TDRE が 0 になったら、要求がきたということ
    return sci_register.get_ssr_tdre() == false;
}

void Sci::process_send_request() {
    if (send_requested()) {
        // データは TDR に入っている
        uint8_t data = sci_register.get_tdr();

        // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
        putc(data, stdout);

        // 送信が終わったらSSR_TDRE を 1 にして送信可能を通知
        sci_register.set_ssr_tdre(true);
    }
}

void Sci::run()
{
    printf("SCI(%d) started\n", index);

    while (!terminate) {
        // 少し動作を遅くする
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // 送信要求がきていたら処理
        process_send_request();
    }
}