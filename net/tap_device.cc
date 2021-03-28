#include "tap_device.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

// todo: mac でもビルドできるようにする
#include <linux/if.h>
#include <linux/if_tun.h>

// todo: クラス構成を変える
// getInterrupt などは rtl8019as に持たせるべき
// レジスタは別のクラスにする

void TAPDevice::prepare()
{
    this->tap_thread[0] = new std::thread(&TAPDevice::run_recv_from_tap, this);
    this->tap_thread[1] = new std::thread(&TAPDevice::run_send_to_tap, this);
    if (this->tap_thread[0] && this->tap_thread[1]) {
        printf("TAP device started\n");
    }
}

bool TAPDevice::createDevice()
{
    struct ifreq ifr;

    this->device_fd = open("/dev/net/tun", O_RDWR);
    if (this->device_fd < 0) {
        fprintf(stderr, "Error: Couldn't create TAP device\n");
        return false;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strncpy(ifr.ifr_name, this->device_name, TAPDevice::DEVICE_NAME_SIZE);

    int err = ioctl(this->device_fd, TUNSETIFF, (void*)&ifr);
    if (err < 0) {
        fprintf(stderr, "Error(%d): Couldn't ioctl device %s\n", errno, ifr.ifr_name);
        close(this->device_fd);
        return false;
    }

    strncpy(this->device_name, ifr.ifr_name, TAPDevice::DEVICE_NAME_SIZE);
    printf("TAP device %s ready\n", this->device_name);

    return true;
}

void TAPDevice::run_recv_from_tap()
{
    // todo: リングバッファへのアクセスは排他制御する
    const uint16_t MAX_ETHERNET_FRAME_SIZE = 1600;
    char buffer[MAX_ETHERNET_FRAME_SIZE];
    const uint8_t PAGE_MAX = 0x80;
    const uint8_t PAGE_MIN = 0x46;

    while (!this->terminate_flag) {
        // TAP からのデータを受け取ったらリングバッファに書き込み、割り込みを入れる

        // RTL8019AS の write ポインタは BNRY、read ポインタは CURR
        // どちらもアドレスではなくリングバッファのページ(256バイト)のインデックスなので注意すること
        // データを書き込むときは、先頭4バイトには制御用データを入れること
        // 0: ステータス
        // 1: 次のページのインデックス
        // 2,3: サイズ(ヘッダを含む)
        int nread = this->read(buffer, sizeof(buffer));
        if (nread < 0) {
            perror("error!\n");
            break;
        }

        printf("Read %d bytes\n", nread);

        uint8_t bnry = BNRY + 1;
        uint16_t address = (uint16_t)bnry * 256;

        printf("BNRY=0x%02x\n", bnry);

        // todo: リングバッファなので、ページの最大数の剰余を取る
        uint8_t next = (nread + 4) / 256 + bnry + 1;
        if (next >= PAGE_MAX) {
            next = (next % PAGE_MAX) + PAGE_MIN;
        }

        printf("NEXT=0x%02x\n", next);
        printf("SIZE= 0x%02x 0x%02x\n", nread & 0xff, nread >> 8);

        // todo: status とは？
        this->saprom[address + 0] = 0x00;
        this->saprom[address + 1] = next;
        // todo: この順番で正しいか確認
        this->saprom[address + 2] = nread & 0xff;
        this->saprom[address + 3] = nread >> 8;

        // todo: リングバッファなので、0番目のページに戻ることに注意しつつ256バイト単位でコピー

        // 1ページ目をコピー
        memcpy(&this->saprom[address + 4], buffer, 256 - 4);

        bnry++;
        if (bnry == PAGE_MAX) {
            bnry = PAGE_MIN;
        }

        // 1ページ目の大きさを減らす
        nread -= 256 - 4;
        char* tmp = &buffer[256 - 4];
        address = (uint16_t)bnry * 256;

        // 2ページ目以降をコピー
        while (nread > 0) {
            memcpy(&this->saprom[address], tmp, 256);

            bnry++;
            if (bnry == PAGE_MAX) {
                bnry = PAGE_MIN;
            }

            nread -= 256;
            tmp += 256;
            address = (uint16_t)bnry * 256;
        }

        BNRY = bnry - 2;

        if (IMR) {
            fprintf(stderr, "IRQ5 interruption set! new BNRY=0x%x\n", BNRY);
            this->hasInterruption = true;
            this->interrupt_cv.notify_all();
        }
    }
}

void TAPDevice::run_send_to_tap()
{

}

TAPDevice::TAPDevice(const char *dev_name, std::condition_variable& interrupt_cv, uint8_t& BNRY, uint8_t& IMR)
    : BNRY(BNRY)
    , IMR(IMR)
    , device_fd(-1)
    , saprom{0}
    , terminate_flag(false)
    , hasInterruption(false)
    , interrupt_cv(interrupt_cv)
{
    strncpy(this->device_name, dev_name, TAPDevice::DEVICE_NAME_SIZE);

    // set mac address in SAPROM
    saprom[0x00] = 0x11;
    saprom[0x02] = 0x22;
    saprom[0x04] = 0x33;
    saprom[0x06] = 0x44;
    saprom[0x08] = 0x55;
    saprom[0x0a] = 0x66;
}

TAPDevice::~TAPDevice()
{
    terminate();
    if (this->device_fd >= 0) {
        ::close(this->device_fd);
    }
    printf("TAP device stopped\n");
}

void TAPDevice::run()
{
    bool ret = createDevice();
    if (ret) {
        this->prepare_thread = new std::thread(&TAPDevice::prepare, this);
    }
}

void TAPDevice::terminate()
{
    terminate_flag = true;

    if (this->prepare_thread) {
        if (this->prepare_thread->joinable()) {
            this->prepare_thread->join();
        }
        delete this->prepare_thread;
    }

    for (int i=0; i < 2; i++) {
        if (tap_thread[i]) {
            if (tap_thread[i]->joinable()) {
                tap_thread[i]->join();
            }
            delete tap_thread[i];
        }
    }
}

interrupt_t TAPDevice::getInterrupt()
{
    // todo: getInterrupt で値を返すだけでは足りない？
    // echo とかで他の割込みを発生させると、IRQ5 が処理される雰囲気がある
    // **interrupt_cv を notify しないといけない！**
    if (this->hasInterruption) {
        fprintf(stderr, "IRQ5: has interrupt\n");
    }
    return this->hasInterruption ? interrupt_t::IRQ5 : interrupt_t::NONE;
}

void TAPDevice::clearInterrupt(interrupt_t type)
{
    fprintf(stderr, "IRQ5: cleared interruption\n");
    this->hasInterruption = false;
}

uint8_t TAPDevice::dma_read(uint16_t address)
{
    return saprom[address];
}

void TAPDevice::dma_write(uint16_t address, uint8_t value)
{
    
}

uint16_t TAPDevice::read(char *buffer, uint16_t size)
{
    printf("READ TO %p\n", buffer);
    return ::read(this->device_fd, buffer, size);
}

uint16_t TAPDevice::write(const char *buffer, uint16_t size)
{
    return ::write(this->device_fd, buffer, size);
}
