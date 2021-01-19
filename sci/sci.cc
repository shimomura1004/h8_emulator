#include <chrono>
#include <fcntl.h>

#include "sci.h"
#include "sci_register.h"

// todo: sci ごとに名前付きパイプを作って、そこに入出力をつなげるほうがよさそう
// todo: シリアルの割り込みを個別に有効にするまでは割り込みをあげてはいけない

// todo: load が開始されるまでに少し待ち時間がある
// todo: (OS側から)送信割り込みが有効化されてない？

void SCI::run_recv_from_h8() {
    while (!terminate_flag) {
        // H8 からデータがくるのを待つ
        // H8 はデータを詰めたあと SSR_TDRE を 0 にすることで通知してくる

        sci_register.wait_tdre_to_be(false);

        // データは TDR に入っている
        uint8_t data = sci_register.get(SCIRegister::SCI::TDR);

        // データを TDR から取得したら SSR_TDRE を 1 にして送信可能を通知
        sci_register.set_bit(SCIRegister::SCI::SSR, SCIRegister::SCI_SSR::TDRE, true);

        // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
        putc(data, stdout);
        fflush(stdout);

        // H8 に送信準備完了の割り込みを発生させる
        if (sci_register.get_bit(SCIRegister::SCI::SCR, SCIRegister::SCI_SCR::TIE)) {
            static interrupt_t TXI_TABLE[] = {
                interrupt_t::TXI0, interrupt_t::TXI1, interrupt_t::TXI2
            };
            interrupt_controller.set(TXI_TABLE[index]);
        }
    }
}

void SCI::run_send_to_h8() {
    while (!terminate_flag) {
        // todo: このロックは必要か？
        int c;
        {
            // デバッガと標準入出力を奪い合わないようにロックする
            // std::lock_guard<std::mutex> lock(mutex);

            c = getchar();
            if (c == EOF) {
                break;
            }
        }

        // H8 が受信するまで待つ
        sci_register.wait_rdrf_to_be(false);

        // H8 に渡すデータは RDR に書き込んでおく
        sci_register.set(SCIRegister::RDR, (uint8_t)c);

        // RDRF を 1 にして H8 に通知
        sci_register.set_bit(SCIRegister::SSR, SCIRegister::SCI_SSR::RDRF, true);

        // シリアル受信割り込みが有効な場合は割り込みを発生させる
        if (sci_register.get_bit(SCIRegister::SCI::SCR, SCIRegister::SCI_SCR::RIE)) {
            static interrupt_t RXI_TABLE[] = {
                interrupt_t::RXI0, interrupt_t::RXI1, interrupt_t::RXI2
            };
            interrupt_controller.set(RXI_TABLE[index]);
        }
    }
}

SCI::SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex)
    : index(index)
    , terminate_flag(false)
    , mutex(mutex)
    , interrupt_controller(interrupt_controller)
{}

SCI::~SCI()
{
    terminate();
}

void SCI::run() {
    sci_thread[0] = new std::thread(&SCI::run_recv_from_h8, this);
    sci_thread[1] = new std::thread(&SCI::run_send_to_h8, this);
    if (sci_thread[0] && sci_thread[1]) {
        printf("SCI(%d) started\n", index);
    }
}

void SCI::terminate() {
    terminate_flag = true;

    for (int i=0; i < 2; i++) {
        if (sci_thread[i]) {
            if (sci_thread[i]->joinable()) {
                sci_thread[i]->join();
            }
            delete sci_thread[i];
        }
    }
}

void SCI::dump(FILE* fp)
{
    sci_register.dump(fp);
}

// H8 上で動くアプリからはこちらの API で SCI にアクセスされる
uint8_t SCI::read(uint32_t address)
{
    return sci_register.read(address);
}

void SCI::write(uint32_t address, uint8_t value)
{
    sci_register.write(address, value);
}
