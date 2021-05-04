#include "rtl8019as.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tap_device.h"

// todo: リングバッファへのアクセスは排他制御する

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
    } else {
        fprintf(stderr, "Error: network is unavailable\n");
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

    while (!this->terminate_flag) {
        // TAP からのデータを受け取ったらリングバッファに書き込み、割り込みを入れる
        // CURR の位置の PROM の値を更新し、CURR を進めるのがデバイス側の責務

        // RTL8019AS の read ポインタは BNRY、write ポインタは CURR
        // BNRY: この番号までは読み終えている、次回はこの番号の次から読む
        // CURR: 次にパケットを受信したら、この番号から書く

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

        uint8_t curr = this->rtl8019as_register.get_CURR();
        uint16_t address = (uint16_t)curr * 256;
        uint16_t head_address = address;

        uint8_t pstart = this->rtl8019as_register.get_PSTART();
        uint8_t pstop = this->rtl8019as_register.get_PSTOP();

        // 管理用の4バイト分を進める
        uint16_t pos = address + 4;
        for (int i = 0; i < nread; i++) {
            if (pos == pstop * 256) {
                pos = pstart * 256;
            }
            this->saprom[pos++] = buffer[i];
        }
        uint8_t next_page = pos / 256 + 1;
        if (next_page >= pstop) {
            next_page -= (pstop - pstart);
        }

        // todo: status とは？
        this->saprom[head_address + 0] = 0x00;
        this->saprom[head_address + 1] = next_page;
        // サイズは管理用の4バイト込みの大きさを書き込む
        this->saprom[head_address + 2] = (nread + 4) & 0xff;
        this->saprom[head_address + 3] = (nread + 4) >> 8;

        // データを書き込んだので CURR を進める
        this->rtl8019as_register.set_CURR(next_page);

        if (this->rtl8019as_register.get_IMR()) {
            this->hasInterruption = true;
            this->interrupt_cv.notify_all();
        }
    }
}

void RTL8019AS::run_send_to_tap()
{
    while (!this->terminate_flag) {
        // TXP がセットされたら、送信処理を開始
        this->rtl8019as_register.wait_txp_to_be(true);

        // TAP デバイスに書き込んで Ethernet フレームを送信
        uint16_t tbcr = this->rtl8019as_register.get_TBCR();
        uint8_t tpsr = this->rtl8019as_register.get_TPSR();

        printf("Send data from 0x%04x, %dbytes\n", tpsr * 256, tbcr);
        ::write(this->device_fd, &this->saprom[tpsr * 256], tbcr);

        // TXP をクリアして処理完了を通知
        this->rtl8019as_register.set_CR_TXP(false);
    }
}

// Remote DMA を開始するときに指定されたサイズだけデータが書かれたら/読まれたら完了
// 完了したら ISR_RDC を true にする
uint8_t RTL8019AS::dma_read(uint16_t address)
{
    uint16_t remote_address = this->rtl8019as_register.get_RSAR();

    uint8_t value = this->saprom[remote_address];
    // fprintf(stderr, "remote read from 0x%x, get 0x%x\n", remote_address, value);

    remote_address++;
    this->rtl8019as_register.set_RSAR(remote_address);

    return value;
}

void RTL8019AS::dma_write(uint16_t address, uint8_t value)
{
    uint16_t remote_address = this->rtl8019as_register.get_RSAR();

    this->saprom[remote_address] = value;
    // fprintf(stderr, "remote write to 0x%x, set 0x%x, then 0x%x\n", remote_address, value, saprom[remote_address]);

    remote_address++;
    this->rtl8019as_register.set_RSAR(remote_address);
}

void RTL8019AS::dump(FILE* fp)
{
}
