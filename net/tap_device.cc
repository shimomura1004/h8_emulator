#include "tap_device.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

TAPDevice::TAPDevice(const char *dev_name)
    : device_fd(-1)
{
    strncpy(this->device_name, dev_name, TAPDevice::DEVICE_NAME_SIZE);
}

TAPDevice::~TAPDevice()
{
    if (this->device_fd >= 0) {
        ::close(this->device_fd);
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

uint16_t TAPDevice::read(char *buffer, uint16_t size)
{
    return ::read(this->device_fd, buffer, size);
}

uint16_t TAPDevice::write(const char *buffer, uint16_t size)
{
    return ::write(this->device_fd, buffer, size);
}
