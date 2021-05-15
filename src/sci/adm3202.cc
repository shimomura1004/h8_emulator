#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "adm3202.h"

// TODO: たまに入力を取りこぼす
// TODO: 最初に割込みを有効にした瞬間にゴミが出力されるが、正しい挙動か？
// SCI TSR の仕様を読むと、TDRE=1 のときは TDR から TSR にコピーされないとのこと
// つまり起動直後(TDRE=1)のゴミ値は TSR にコピーされず、送信されないということ
// その後は？

const interrupt_t ADM3202::TXI_TABLE[3] = {
    interrupt_t::TXI0, interrupt_t::TXI1, interrupt_t::TXI2
};

const interrupt_t ADM3202::RXI_TABLE[3] = {
    interrupt_t::RXI0, interrupt_t::RXI1, interrupt_t::RXI2
};

bool ADM3202::open_sci_socket()
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

void ADM3202::prepare()
{
    if (!this->open_sci_socket()) {
        return;
    }

    this->sci_thread[0] = new std::thread(&ADM3202::run_recv_from_h8, this);
    this->sci_thread[1] = new std::thread(&ADM3202::run_send_to_h8, this);
    if (this->sci_thread[0] && this->sci_thread[1]) {
        printf("SCI(%d) started\n", this->index);
    }
}

void ADM3202::run_recv_from_h8() {
    while (!terminate_flag) {
        // H8 からデータがくるのを待つ
        // H8 はデータを詰めたあと SSR_TDRE を 0 にすることで通知してくる
        this->adm3202_registers->wait_tdre_to_be(false);

        // データは TDR に入っている
        uint8_t data = this->adm3202_registers->get(ADM3202_Registers::SCI::TDR);

        // データを TDR から取得したら SSR_TDRE を 1 にして送信可能を通知
        this->adm3202_registers->set_bit(ADM3202_Registers::SCI::SSR, ADM3202_Registers::SCI_SSR::TDRE, true);

        // 送信
        if (!this->use_stdio) {
            ::write(this->sci_sock_fd, &data, sizeof(char));
        } else {
            ::write(1, &data, sizeof(char));
        }

        // H8 に送信準備完了の割り込みを発生させる
        if (this->adm3202_registers->get_bit(ADM3202_Registers::SCI::SCR, ADM3202_Registers::SCI_SCR::TIE)) {
            this->hasTxiInterruption = true;

            // 割込みを通知する
            interrupt_cv.notify_all();
        }
    }
}

void ADM3202::run_send_to_h8() {
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
        this->adm3202_registers->wait_rdrf_to_be(false);

        // H8 に渡すデータは RDR に書き込んでおく
        this->adm3202_registers->set(ADM3202_Registers::RDR, (uint8_t)c);

        // RDRF を 1 にして H8 に通知
        this->adm3202_registers->set_bit(ADM3202_Registers::SSR, ADM3202_Registers::SCI_SSR::RDRF, true);

        // シリアル受信割り込みが有効な場合は割り込みを発生させる
        if (this->adm3202_registers->get_bit(ADM3202_Registers::SCI::SCR, ADM3202_Registers::SCI_SCR::RIE)) {
            this->hasRxiInterruption = true;

            // 割込みを通知する
            interrupt_cv.notify_all();
        }
    }
}

ADM3202::ADM3202(uint8_t index, std::condition_variable& interrupt_cv, bool use_stdio)
    : use_stdio(use_stdio)
    , index(index)
    , terminate_flag(false)
    , adm3202_registers(new ADM3202_Registers)
    , interrupt_cv(interrupt_cv)
    , hasTxiInterruption(false)
    , hasRxiInterruption(false)
{}

ADM3202::~ADM3202()
{
    terminate();
    close(this->sci_socket);
    close(this->sci_sock_fd);
    delete this->adm3202_registers;
    printf("SCI(%d) stopped\n", index);
}

void ADM3202::run() {
    this->prepare_thread = new std::thread(&ADM3202::prepare, this);
}

void ADM3202::terminate() {
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

interrupt_t ADM3202::getInterrupt()
{
    if (this->hasTxiInterruption) {
        return ADM3202::TXI_TABLE[this->index];
    } else if (this->hasRxiInterruption) {
        return ADM3202::RXI_TABLE[this->index];
    } else {
        return interrupt_t::NONE;
    }
}

void ADM3202::clearInterrupt(interrupt_t type)
{
    if (type == ADM3202::TXI_TABLE[this->index]) {
        if (this->hasTxiInterruption) {
            this->hasTxiInterruption = false;
        } else {
            fprintf(stderr, "Error: SCI(%d) does not generate TXI%d\n", this->index, this->index);
        }
    } else if (type == ADM3202::RXI_TABLE[this->index]) {
        if (this->hasRxiInterruption) {
            this->hasRxiInterruption = false;
        } else {
            fprintf(stderr, "Error: SCI(%d) does not generate RXI%d\n", this->index, this->index);
        }
    } else {
        fprintf(stderr, "Error: SCI(%d) does not generate interruption: %d\n", this->index, type);
    }
}

void ADM3202::dump(FILE* fp)
{
    this->adm3202_registers->dump(fp);
}

// H8 上で動くアプリからはこちらの API で SCI にアクセスされる
uint8_t ADM3202::read(uint32_t address)
{
    return this->adm3202_registers->read(address);
}

void ADM3202::write(uint32_t address, uint8_t value)
{
    this->adm3202_registers->write(address, value);
}
