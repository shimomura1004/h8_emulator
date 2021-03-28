#include "rtl8019as.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tap_device.h"

// todo: readme に使い方を書く
//   $ sudo ip tuntap add dev tun0 mode tun user shimo
//   $ sudo ip addr add 10.0.0.2/24 dev tun0
//   $ sudo ip link set tun0 up
//   $ h8300h kzload.elf

// todo: リングバッファへのアクセスは排他制御する

void debug(uint8_t* buffer, int size)
{
    printf("vvvv\n");
    for (int i=0; i < size; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%02x ", buffer[i]);
    }
    printf("\n^^^^\n");
}

RTL8019AS::RTL8019AS(std::condition_variable& interrupt_cv)
    : device_fd(-1)
    , saprom{0}
    , terminate_flag(false)
    , hasInterruption(false)
    , interrupt_cv(interrupt_cv)
{
    strncpy(this->device_name, "tap0", DEVICE_NAME_SIZE);

    // set mac address in SAPROM
    saprom[0x00] = 0x00;
    saprom[0x02] = 0x11;
    saprom[0x04] = 0x22;
    saprom[0x06] = 0x33;
    saprom[0x08] = 0x44;
    saprom[0x0a] = 0x55;
}

RTL8019AS::~RTL8019AS()
{
    terminate();
    if (this->device_fd >= 0) {
        ::close(this->device_fd);
        printf("TAP device stopped\n");
    }
}

void RTL8019AS::run()
{
    this->device_fd = TAPDevice::create(this->device_name, DEVICE_NAME_SIZE);
    if (this->device_fd >= 0) {
        this->prepare_thread = new std::thread(&RTL8019AS::prepare, this);
    }
}

void RTL8019AS::terminate()
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

uint8_t RTL8019AS::read8(uint32_t address)
{
    if (address <= 0x0f) {
        // Register access
        return this->rtl8019as_register.read8(address);
    } else if (address <= 0x17) {
        // DMA port
        return this->dma_read(address);
    } else if (address <= 0x1f) {
        // reset port
        return 0;
    } else {
        fprintf(stderr, "Error: Invalid access to RTL8019AS register (0x%x)", address);
        return 0;
    }
}

void RTL8019AS::write8(uint32_t address, uint8_t value)
{
    if (address <= 0x0f) {
        // Register access
        return this->rtl8019as_register.write8(address, value);
    } else if (address <= 0x17) {
        // DMA port
        return this->dma_write(address, value);
    } else if (address <= 0x1f) {
        // reset port
        return;
    } else {
        fprintf(stderr, "Error: Invalid access to RTL8019AS register (0x%x)", address);
        return;
    }
}

interrupt_t RTL8019AS::getInterrupt()
{
    return this->hasInterruption ? interrupt_t::IRQ5 : interrupt_t::NONE;
}

void RTL8019AS::clearInterrupt(interrupt_t type)
{
    this->hasInterruption = false;
}

void RTL8019AS::prepare()
{
    this->tap_thread[0] = new std::thread(&RTL8019AS::run_recv_from_tap, this);
    this->tap_thread[1] = new std::thread(&RTL8019AS::run_send_to_tap, this);
    if (this->tap_thread[0] && this->tap_thread[1]) {
        printf("TAP device started\n");
    }
}

void RTL8019AS::run_recv_from_tap()
{
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
        int nread = ::read(this->device_fd, buffer, sizeof(buffer));
        if (nread < 0) {
            perror("error!\n");
            break;
        }

        printf("Read %d bytes\n", nread);

        uint8_t bnry = this->rtl8019as_register.get_BNRY() + 1;
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
        // 4バイト分の管理データ分を加算する
        this->saprom[address + 2] = (nread + 4) & 0xff;
        this->saprom[address + 3] = (nread + 4) >> 8;

        // todo: リングバッファなので、0番目のページに戻ることに注意しつつ256バイト単位でコピー

        // 1ページ目をコピー
        // RTL8019AS では受信したフレームに対し、先頭に4バイト分の管理データを保持するので、
        // 4バイト目以降にフレームをコピーする
        address += 4;
        memcpy(&this->saprom[address], buffer, std::min(256 - 4, nread));
        debug(&this->saprom[address], std::min(256 - 4, nread));

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
            memcpy(&this->saprom[address], tmp, std::min(256, nread));

            bnry++;
            if (bnry == PAGE_MAX) {
                bnry = PAGE_MIN;
            }

            nread -= 256;
            tmp += 256;
            address = (uint16_t)bnry * 256;
        }

        this->rtl8019as_register.set_BNRY(bnry - 2);

        if (this->rtl8019as_register.get_IMR()) {
            fprintf(stderr, "IRQ5 interruption set! new BNRY=0x%x\n", this->rtl8019as_register.get_BNRY());
            this->hasInterruption = true;
            this->interrupt_cv.notify_all();
        }
    }
}

void RTL8019AS::run_send_to_tap()
{

}

// todo: CURR を動かさないといけない
uint8_t RTL8019AS::dma_read(uint16_t address)
{
    uint16_t remote_address = this->rtl8019as_register.get_RSAR();

    uint8_t value = saprom[remote_address];
    fprintf(stderr, "remote read from 0x%x, get 0x%x\n", remote_address, value);

    remote_address++;
    this->rtl8019as_register.set_RSAR(remote_address);

    return value;
}

void RTL8019AS::dma_write(uint16_t address, uint8_t value)
{
    uint16_t remote_address = this->rtl8019as_register.get_RSAR();

    saprom[remote_address] = value;
    fprintf(stderr, "remote write to 0x%x, set 0x%x\n", remote_address, value);

    remote_address++;
    this->rtl8019as_register.set_RSAR(remote_address);
}

void RTL8019AS::dump(FILE* fp)
{
}
