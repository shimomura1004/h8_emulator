#include <chrono>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "sci.h"
#include "sci_register.h"

// todo: sci ごとに名前付きパイプを作って、そこに入出力をつなげるほうがよさそう -> socket にする
// todo: シリアルの割り込みを個別に有効にするまでは割り込みをあげてはいけない

// todo: load が開始されるまでに少し待ち時間がある
// todo: (OS側から)送信割り込みが有効化されてない？


bool SCI::open_sci_socket()
{
    sprintf(this->sci_sock_name, "ser%d", this->index);

    this->sci_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (this->sci_socket == -1)
    {
        fprintf(stderr, "Error: Failed to create socket.\n");
        return false;
    }

    struct sockaddr_un sa = {0};
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, this->sci_sock_name);

    remove(sa.sun_path);

    if (bind(this->sci_socket, (struct sockaddr*) &sa, sizeof(struct sockaddr_un)) == -1)
    {
        fprintf(stderr, "Error: could not bind socket.\n");
        close(this->sci_socket);
        return false;
    }

    if (listen(this->sci_socket, 128) == -1)
    {
        fprintf(stderr, "Error: could not listen.\n");
        close(this->sci_socket);
        return false;
    }

    this->sci_sock_fd = accept(this->sci_socket, NULL, NULL);
    if (this->sci_sock_fd == -1)
    {
        fprintf(stderr, "Error: could not accept connection.\n");
        close(this->sci_socket);
        return false;
    }

    return true;
}

void SCI::run_recv_from_h8() {
    // // ソケットを開くのはこちらで
    // if (!open_sci_socket()) {
    //     return;
    // }

    while (!terminate_flag) {
        // H8 からデータがくるのを待つ
        // H8 はデータを詰めたあと SSR_TDRE を 0 にすることで通知してくる
        sci_register.wait_tdre_to_be(false);

        // データは TDR に入っている
        uint8_t data = sci_register.get(SCIRegister::SCI::TDR);

        // データを TDR から取得したら SSR_TDRE を 1 にして送信可能を通知
        sci_register.set_bit(SCIRegister::SCI::SSR, SCIRegister::SCI_SSR::TDRE, true);

        // 送信(シリアルポートがターミナルに接続されているとして、標準出力に出力)
        ::write(this->sci_sock_fd, &data, sizeof(char));
fputc(data, stdout);
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
    // ソケットを開くまでは待ち合わせる
    while (!terminate_flag) {
        // todo: このロックは必要か？
        // int c;
        char c;
        {
            // デバッガと標準入出力を奪い合わないようにロックする
            // std::lock_guard<std::mutex> lock(mutex);

            // c = fgetc(this->sci_sock);
            ::read(this->sci_sock_fd, &c, sizeof(char));

            // if (c == EOF) {
            //     // 読み取りに失敗したら開き直してみる
            //     // fclose(this->sci_sock);

            //     // todo: 再接続(複数接続)に対応する
            //     // bool ret = open_pipe(this->rx_pipe_name, this->rx, RX);
            //     // if (!ret) {
            //     //     break;
            //     // }

            //     return;
            // }
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
    : SCI(index, interrupt_controller, mutex, nullptr, nullptr)
{}

SCI::SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex, FILE* tx, FILE* rx)
    : index(index)
    , terminate_flag(false)
    , mutex(mutex)
    , interrupt_controller(interrupt_controller)
{
}

SCI::~SCI()
{
    terminate();
    // fclose(this->sci_sock);
    printf("SCI(%d) stopped\n", index);
}

void SCI::run() {
    if (this->index != 1)
    return;
    if (!open_sci_socket()) {
        return;
    }

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
