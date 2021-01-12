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

static FILE* fp;
Sci::Sci(uint8_t index, InnerMemory& memory, bool& terminate, std::mutex& mutex)
    : index(index)
    , terminate(terminate)
    , mutex(mutex)
    , sci_register(index, memory)
{
	timeout.tv_sec = 0; 
	timeout.tv_usec = 0;

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

// void Sci::process_recv_request()
// {
//     // デバッガと標準入出力を奪い合わないようにロックする
//     std::lock_guard<std::mutex> lock(mutex);

//     // select が fdset をクリアするので毎回設定する必要がある
//     FD_ZERO(&fdset);
// 	FD_SET(0, &fdset);

//     int ret = select(0 + 1, &fdset , NULL , NULL , &timeout);
//     if (ret == 1) {
//         // 標準入力にデータがある
//         if (FD_ISSET(0, &fdset)) {
//             int c;
//             while (1) {
//                 c = fgetc(stdin);
// // printf("[%c(%02x)]\n", c, c);
//                 // todo: EOF がハンドルができていない
//                 if (c == EOF) {
//                     // buffer.push(0x0a);
//                     break;
//                 }

// // todo: OS バイナリを受信しているときは改行コードで終了してはいけない
//                 buffer.push(c);
//                 // if (buffer.back() == 0x0a) {
//                 //     break;
//                 // }
//             }
//         }
//     }

//     // まだ H8 が処理していないので何もしない
//     if (sci_register.get_ssr_rdrf()) {
//         return;
//     }

//     // バッファにデータがあるのであれば H8 に送信
//     if (!buffer.empty()) {
//         // todo: たまにコマンドをうまくひろえない
//         // 標準入力からの受け取りに漏れはない、 H8 に渡すときに漏れている
//         // スレッド間のタイミングの問題に思える

//         // H8 に渡すデータは RDR に書き込んでおく
//         sci_register.set_rdr(buffer.front());

//         // RDRF を 1 にして H8 に通知
//         sci_register.set_ssr_rdrf(true);

//         buffer.pop();
//     }
// }

// void Sci::process_recv_request()
// {
//     // デバッガと標準入出力を奪い合わないようにロックする
//     std::lock_guard<std::mutex> lock(mutex);

//     while (1) {
//         // select が fdset をクリアするので毎回設定する必要がある
//         FD_ZERO(&fdset);
//         FD_SET(0, &fdset);

//         int ret = select(0 + 1, &fdset , NULL , NULL , &timeout);
//         if (ret == 1) {
//             // 標準入力にデータがある
//             int c;
//             while (1) {
//                 c = fgetc(stdin);
// printf("[%c(%02x)]\n", c, c);
//                 // todo: EOF がハンドルができていない
//                 if (c == EOF) {
//                     // buffer.push(0x0a);
//                     break;
//                 }

// // todo: OS バイナリを受信しているときは改行コードで終了してはいけない
//                 buffer.push(c);
//                 if (buffer.back() == 0x0a) {
//                     break;
//                 }
//             }
//         } else {
//             break;
//         }
//     }

//     // まだ H8 が処理していないので何もしない
//     if (sci_register.get_ssr_rdrf()) {
//         return;
//     }

//     // バッファにデータがあるのであれば H8 に送信
//     if (!buffer.empty()) {
//         // todo: たまにコマンドをうまくひろえない
//         // 標準入力からの受け取りに漏れはない、 H8 に渡すときに漏れている
//         // スレッド間のタイミングの問題に思える

//         // H8 に渡すデータは RDR に書き込んでおく
//         sci_register.set_rdr(buffer.front());

//         // RDRF を 1 にして H8 に通知
//         sci_register.set_ssr_rdrf(true);

//         buffer.pop();
//     }
// }

void Sci::process_recv_request()
{
    // デバッガと標準入出力を奪い合わないようにロックする
    std::lock_guard<std::mutex> lock(mutex);

    int c;
    while (1) {
        c = getchar();
// printf("[%c(%02x)]\n", c, c);
        // todo: EOF がハンドルができていない
        // todo: OS バイナリを受信しているときは EOF で終了してはいけない
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
        // todo: たまにコマンドをうまくひろえない
        // 標準入力からの受け取りに漏れはない、 H8 に渡すときに漏れている
        // スレッド間のタイミングの問題に思える

        // H8 に渡すデータは RDR に書き込んでおく
        sci_register.set_rdr(buffer.front());

        // RDRF を 1 にして H8 に通知
        sci_register.set_ssr_rdrf(true);

        buffer.pop();
    }
}

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