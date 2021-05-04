#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "sci.h"
#include "sci_registers.h"

// todo: たまに入力を取りこぼす
// todo: 最初に割込みを有効にした瞬間にゴミが出力されるが、正しい挙動か？
// SCI TSR の仕様を読むと、TDRE=1 のときは TDR から TSR にコピーされないとのこと
// つまり起動直後(TDRE=1)のゴミ値は TSR にコピーされず、送信されないということ
// その後は？

const interrupt_t H8300H_SCI::TXI_TABLE[3] = {
    interrupt_t::TXI0, interrupt_t::TXI1, interrupt_t::TXI2
};

const interrupt_t H8300H_SCI::RXI_TABLE[3] = {
    interrupt_t::RXI0, interrupt_t::RXI1, interrupt_t::RXI2
};

bool H8300H_SCI::open_sci_socket()
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

void H8300H_SCI::prepare()
{
    if (!this->open_sci_socket()) {
        return;
    }

    this->sci_thread[0] = new std::thread(&H8300H_SCI::run_recv_from_h8, this);
    this->sci_thread[1] = new std::thread(&H8300H_SCI::run_send_to_h8, this);
    if (this->sci_thread[0] && this->sci_thread[1]) {
        printf("SCI(%d) started\n", this->index);
    }
}

void H8300H_SCI::run_recv_from_h8() {
    while (!terminate_flag) {
        // H8 からデータがくるのを待つ
        // H8 はデータを詰めたあと SSR_TDRE を 0 にすることで通知してくる
        sci_registers.wait_tdre_to_be(false);

        // データは TDR に入っている
        uint8_t data = sci_registers.get(H8300H_SCI_Registers::SCI::TDR);

        // データを TDR から取得したら SSR_TDRE を 1 にして送信可能を通知
        sci_registers.set_bit(H8300H_SCI_Registers::SCI::SSR, H8300H_SCI_Registers::SCI_SSR::TDRE, true);

        // 送信
        if (!this->use_stdio) {
            ::write(this->sci_sock_fd, &data, sizeof(char));
        } else {
            ::write(1, &data, sizeof(char));
        }

        // H8 に送信準備完了の割り込みを発生させる
        if (sci_registers.get_bit(H8300H_SCI_Registers::SCI::SCR, H8300H_SCI_Registers::SCI_SCR::TIE)) {
            this->hasTxiInterruption = true;

            // 割込みを通知する
            interrupt_cv.notify_all();
        }
    }
}

void H8300H_SCI::run_send_to_h8() {
    while (!terminate_flag) {
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

            // 読み取りに失敗したら再度ソケットを開く
            close(this->sci_socket);
            close(this->sci_sock_fd);
            if (!open_sci_socket()) {
                break;
            }
            continue;
        }

        // H8 が受信するまで待つ
        sci_registers.wait_rdrf_to_be(false);

        // H8 に渡すデータは RDR に書き込んでおく
        sci_registers.set(H8300H_SCI_Registers::RDR, (uint8_t)c);

        // RDRF を 1 にして H8 に通知
        sci_registers.set_bit(H8300H_SCI_Registers::SSR, H8300H_SCI_Registers::SCI_SSR::RDRF, true);

        // シリアル受信割り込みが有効な場合は割り込みを発生させる
        if (sci_registers.get_bit(H8300H_SCI_Registers::SCI::SCR, H8300H_SCI_Registers::SCI_SCR::RIE)) {
            this->hasRxiInterruption = true;

            // 割込みを通知する
            interrupt_cv.notify_all();
        }
    }
}

H8300H_SCI::H8300H_SCI(uint8_t index, std::condition_variable& interrupt_cv, bool use_stdio)
    : use_stdio(use_stdio)
    , index(index)
    , terminate_flag(false)
    , interrupt_cv(interrupt_cv)
    , hasTxiInterruption(false)
    , hasRxiInterruption(false)
{}

H8300H_SCI::~H8300H_SCI()
{
    terminate();
    close(this->sci_socket);
    close(this->sci_sock_fd);
    printf("SCI(%d) stopped\n", index);
}

void H8300H_SCI::run() {
    this->prepare_thread = new std::thread(&H8300H_SCI::prepare, this);
}

void H8300H_SCI::terminate() {
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

interrupt_t H8300H_SCI::getInterrupt()
{
    if (this->hasTxiInterruption) {
        return H8300H_SCI::TXI_TABLE[this->index];
    } else if (this->hasRxiInterruption) {
        return H8300H_SCI::RXI_TABLE[this->index];
    } else {
        return interrupt_t::NONE;
    }
}

void H8300H_SCI::clearInterrupt(interrupt_t type)
{
    if (type == H8300H_SCI::TXI_TABLE[this->index]) {
        if (this->hasTxiInterruption) {
            this->hasTxiInterruption = false;
        } else {
            fprintf(stderr, "Error: SCI(%d) does not generate TXI%d\n", this->index, this->index);
        }
    } else if (type == H8300H_SCI::RXI_TABLE[this->index]) {
        if (this->hasRxiInterruption) {
            this->hasRxiInterruption = false;
        } else {
            fprintf(stderr, "Error: SCI(%d) does not generate RXI%d\n", this->index, this->index);
        }
    } else {
        fprintf(stderr, "Error: SCI(%d) does not generate interruption: %d\n", this->index, type);
    }
}

void H8300H_SCI::dump(FILE* fp)
{
    sci_registers.dump(fp);
}

// H8 上で動くアプリからはこちらの API で SCI にアクセスされる
uint8_t H8300H_SCI::read(uint32_t address)
{
    return sci_registers.read(address);
}

void H8300H_SCI::write(uint32_t address, uint8_t value)
{
    sci_registers.write(address, value);
}
