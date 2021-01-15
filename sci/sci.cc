#include <chrono>
#include <fcntl.h>

#include "sci.h"
#include "sci_register.h"

// todo: sci ごとに名前付きパイプを作って、そこに入出力をつなげるほうがよさそう
// todo: シリアルの割り込みを個別に有効にするまでは割り込みをあげてはいけない

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

        // シリアル受信割り込みが有効な場合は割り込みを発生させる
        if (sci_register.get_scr_rie()) {
            static interrupt_t RXI_TABLE[] = {
                interrupt_t::RXI0, interrupt_t::RXI1, interrupt_t::RXI2
            };
            interrupt_controller.set(RXI_TABLE[index]);
        }
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

        // 割り込みを発生させる
        interrupt_controller.set(interrupt_t::TXI1);
    }
}

void Sci::run()
{
    printf("SCI(%d) started\n", index);

    while (!terminate) {
        // todo: ビジーループがつらいのでイベントドリブンにする
        // 送信・受信をスレッドを2つにわけたらブロッキングで実現できるのでは？

        // 少し動作を遅くする
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // 送信要求がきていたら処理
        process_send_request();

        // (H8 の SCI1 がつながっている)標準入力からデータがきたら処理する
        process_recv_request();
    }
}

Sci::Sci(uint8_t index, InnerMemory& memory, InterruptController& interrupt_controller, bool& terminate, std::mutex& mutex)
    : index(index)
    , terminate(terminate)
    , mutex(mutex)
    , sci_register(index, memory)
    , interrupt_controller(interrupt_controller)
{
    sci_thread = new std::thread(&Sci::run, this);

    int flags;
    flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
}

Sci::~Sci()
{
    if (sci_thread) {
        if (sci_thread->joinable()) {
            sci_thread->join();
        }
        delete sci_thread;
    }
}