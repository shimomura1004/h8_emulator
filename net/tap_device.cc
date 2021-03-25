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
    // test
    const uint16_t BUFFER_SIZE = 10000;
    char buffer[BUFFER_SIZE];
    unsigned char ip[4];

    while (this->terminate_flag) {
        int nread = this->read(buffer, sizeof(buffer));
        if (nread < 0) {
            perror("error!\n");
            break;
        }

        printf("Read %d bytes\n", nread);

        memcpy(ip, &buffer[12], 4);
        memcpy(&buffer[12], &buffer[16], 4);
        memcpy(&buffer[16], ip, 4);

        buffer[20] = 0;
        *((unsigned short *)&buffer[22]) += 8;
            
        nread = this->write(buffer, nread);
    
        printf("Write %d bytes to tun/tap device\n", nread);
  }
}

void TAPDevice::run_send_to_tap()
{

}

TAPDevice::TAPDevice(const char *dev_name)
    : device_fd(-1)
    , terminate_flag(false)
{
    strncpy(this->device_name, dev_name, TAPDevice::DEVICE_NAME_SIZE);
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
    return interrupt_t::NONE;
}

void TAPDevice::clearInterrupt(interrupt_t type)
{
}

uint16_t TAPDevice::read(char *buffer, uint16_t size)
{
    return ::read(this->device_fd, buffer, size);
}

uint16_t TAPDevice::write(const char *buffer, uint16_t size)
{
    return ::write(this->device_fd, buffer, size);
}
