#ifndef _SCI_INCLUDED_
#define _SCI_INCLUDED_

#include <sys/select.h>
#include <thread>
#include <mutex>
#include <queue>
#include "sci_register.h"
#include "interrupt/interrupt_controller.h"

class Sci {
private:
    std::thread* sci_thread;

    uint8_t index;
    bool& terminate;
    std::mutex& mutex;
    SciRegister sci_register;
    InterruptController& interrupt_controller;

    std::queue<char> buffer;

    bool send_requested();
    void process_recv_request();
    void process_send_request();

public:
    Sci(uint8_t index, InnerMemory& memory, InterruptController& interrupt_controller, bool& terminate, std::mutex& mutex);
    ~Sci();
    void run();

};

class SCI {
private:
    std::thread* sci_thread[2];
    uint8_t index;
    bool terminate_flag;
    std::mutex& mutex;
    SCIRegister sci_register;
    InterruptController& interrupt_controller;

    void run_recv_from_h8() {
        while (!terminate_flag) {
            // H8 からデータがくるのを待つ
            sci_register.wait_tdre();

            // データは TDR に入っている
            uint8_t data = sci_register.get_tdr();

            // 送信が終わったらSSR_TDRE を 1 にして送信可能を通知
            sci_register.set_ssr_tdre(true);

            // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
            putc(data, stdout);
            fflush(stdout);

            // 割り込みを発生させる
            // todo: index に応じた処理
            interrupt_controller.set(interrupt_t::TXI1);
        }
    }

    void run_send_to_h8() {
        while (!terminate_flag) {
            // デバッガと標準入出力を奪い合わないようにロックする
            std::lock_guard<std::mutex> lock(mutex);

            int c = getchar();
            if (c == EOF) {
                break;
            }

            // H8 が受信するまで待つ
            sci_register.wait_rdrf();

            // H8 に渡すデータは RDR に書き込んでおく
            sci_register.set_rdr(c);

            // RDRF を 1 にして H8 に通知
            sci_register.set_ssr_rdrf(true);

            // シリアル受信割り込みが有効な場合は割り込みを発生させる
            if (sci_register.get_scr_rie()) {
                static interrupt_t RXI_TABLE[] = {
                    interrupt_t::RXI0, interrupt_t::RXI1, interrupt_t::RXI2
                };
                interrupt_controller.set(RXI_TABLE[index]);
            }
        }
    }

public:
    SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex)
        : index(index)
        , terminate_flag(false)
        , mutex(mutex)
        , interrupt_controller(interrupt_controller)
    {}
    ~SCI()
    {
        terminate();
    }

    void run() {
        sci_thread[0] = new std::thread(&SCI::run_recv_from_h8, this);
        sci_thread[1] = new std::thread(&SCI::run_send_to_h8, this);
        if (sci_thread[0] && sci_thread[1]) {
            printf("SCI(%d) started\n", index);
        }
    }

    void terminate() {
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

    // H8 のアプリコードからアクセスされる
    uint8_t read(uint32_t address) { return sci_register.read(address); }
    void write(uint32_t address, uint8_t value) { sci_register.write(address, value); }

};

#endif
