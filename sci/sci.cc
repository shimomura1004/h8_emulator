#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "sci.h"
#include "sci_register.h"

// todo: シリアルの割り込みを個別に有効にするまでは割り込みをあげてはいけない

// todo: load が開始されるまでに少し待ち時間がある
// todo: (OS側から)送信割り込みが有効化されてない？


bool SCI::open_sci_socket()
{
    if (this->use_stdio) {
        this->sci_sock_fd = 0;
        return true;
    }

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

    fprintf(stderr, "Info: %s connected.\n", this->sci_sock_name);

    return true;
}

void SCI::prepare()
{
    if (!this->open_sci_socket()) {
        return;
    }

    this->sci_thread[0] = new std::thread(&SCI::run_recv_from_h8, this);
    this->sci_thread[1] = new std::thread(&SCI::run_send_to_h8, this);
    if (this->sci_thread[0] && this->sci_thread[1]) {
        printf("SCI(%d) started\n", this->index);
    }
}

void SCI::run_recv_from_h8() {
    while (!terminate_flag) {
        // H8 からデータがくるのを待つ
        // H8 はデータを詰めたあと SSR_TDRE を 0 にすることで通知してくる
        sci_register.wait_tdre_to_be(false);

        // データは TDR に入っている
        uint8_t data = sci_register.get(SCIRegister::SCI::TDR);

        // データを TDR から取得したら SSR_TDRE を 1 にして送信可能を通知
        sci_register.set_bit(SCIRegister::SCI::SSR, SCIRegister::SCI_SSR::TDRE, true);

        // 送信
        if (!this->use_stdio) {
            ::write(this->sci_sock_fd, &data, sizeof(char));
        } else {
            ::write(1, &data, sizeof(char));
        }

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
            // デバッガと標準入出力を奪い合わないようにロックする
            // std::lock_guard<std::mutex> lock(mutex);

        char c;
        ssize_t size;
        if (!this->use_stdio) {
            size = ::read(this->sci_sock_fd, &c, sizeof(char));
        } else {
            size = ::read(0, &c, sizeof(char));
        }

        if (size != 1) {
            fprintf(stderr, "Info: socket is closed.\n");

            if (this->use_stdio) {
                exit(1);
            }

            // todo: うまく開けていない？
            // 読み取りに失敗したら開き直してみる
            close(this->sci_socket);
            close(this->sci_sock_fd);
            if (!open_sci_socket()) {
                break;
            }
            continue;
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

SCI::SCI(uint8_t index, InterruptController& interrupt_controller, std::mutex& mutex, bool use_stdio)
    : use_stdio(use_stdio)
    , index(index)
    , terminate_flag(false)
    , mutex(mutex)
    , interrupt_controller(interrupt_controller)
{}

SCI::~SCI()
{
    terminate();
    close(this->sci_socket);
    close(this->sci_sock_fd);
    printf("SCI(%d) stopped\n", index);
}

void SCI::run() {
    this->prepare_thread = new std::thread(&SCI::prepare, this);
}

void SCI::terminate() {
    terminate_flag = true;

    if (this->prepare_thread) {
        if (this->prepare_thread->joinable()) {
            this->prepare_thread->join();
        }
        delete this->prepare_thread;
    }

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
