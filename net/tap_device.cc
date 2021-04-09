#include "tap_device.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#ifdef __linux__
#include <linux/if.h>
#include <linux/if_tun.h>

int TAPDevice::create(char* device_name, int buffer_size)
{
    struct ifreq ifr;

    int device_fd = open("/dev/net/tun", O_RDWR);
    if (device_fd < 0) {
        fprintf(stderr, "Error: Couldn't create TAP device\n");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    strncpy(ifr.ifr_name, device_name, buffer_size);

    int err = ioctl(device_fd, TUNSETIFF, (void*)&ifr);
    if (err < 0) {
        fprintf(stderr, "Error(%d): Couldn't ioctl device %s\n", errno, ifr.ifr_name);
        close(device_fd);
        return -1;
    }

    strncpy(device_name, ifr.ifr_name, buffer_size);
    printf("TAP device %s ready\n", device_name);

    return device_fd;
}
#else
int TAPDevice::create(char* device_name, int buffer_size)
{
    return -1;
}
#endif
