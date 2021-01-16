#ifndef _MCU_INCLUDED_
#define _MCU_INCLUDED_

#include <cstdint>
#include <string>
#include <mutex>
#include "sci/sci.h"

class MCU {
    // モード5(内蔵ROM有効拡張16Mバイトモード)
    static const uint32_t vec_start = 0x000000; // ベクタエリア
    static const uint32_t vec_end   = 0x0000ff;
    static const uint32_t rom_start = 0x000100; // 内蔵 ROM
    static const uint32_t rom_end   = 0x07ffff;
                                                // 外部アドレス空間
                                                // 内部 I/O レジスタ
    static const uint32_t ram_start = 0xffbf00; // 内蔵 RAM
    static const uint32_t ram_end   = 0xffff1f;
    static const uint32_t io_start  = 0xffff20; // 内部 I/O レジスタ
    static const uint32_t io_end    = 0xffffe9;
                                                // 外部アドレス空間
    static const uint32_t all_end   = 0xffffff;

    uint8_t rom[rom_end - rom_start + 1];
    uint8_t ram[ram_end - ram_start + 1];

    static const uint32_t sci0_start = 0xffffb0;
    static const uint32_t sci0_end   = 0xffffb6;
    static const uint32_t sci1_start = 0xffffb8;
    static const uint32_t sci1_end   = 0xffffbe;
    static const uint32_t sci2_start = 0xffffc0;
    static const uint32_t sci2_end   = 0xffffc6;

    SCI sci1;

    std::mutex mutex;

public:
    MCU(InterruptController& interrupt_controller, std::mutex& mutex)
        : sci1(1, interrupt_controller, mutex)
    {
        sci1.run();
    }

template<uint8_t n, class T>
T read(uint32_t address)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (rom_start <= address && address <= rom_end) {
        switch (n) {
        case  8: return rom[address];
        case 16: return __builtin_bswap16(*(uint16_t*)&rom[address]);
        case 32: return __builtin_bswap32(*(uint32_t*)&rom[address]);
        default: break;
        }
    } else if (ram_start <= address && address <= ram_end) {
        switch (n) {
        case  8: return ram[address - ram_start];
        case 16: return __builtin_bswap16(*(uint16_t*)&ram[address - ram_start]);
        case 32: return __builtin_bswap32(*(uint32_t*)&ram[address - ram_start]);
        default: break;
        }
    } else if (sci1_start <= address && address <= sci1_end) {
        if (n == 8) {
            return sci1.read(address - sci1_start);
        }
    }

    fprintf(stderr, "Error: Invalid read access to 0x%06x\n", address);
    return 0;
}

template<uint8_t n, class T>
void write(uint32_t address, T value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (ram_start <= address && address <= ram_end) {
        switch (n) {
        case  8:
            ram[address - ram_start] = value;
            return;
        case 16:
            *(T*)&ram[address - ram_start] = __builtin_bswap16(value);
            return;
        case 32:
            *(T*)&ram[address - ram_start] = __builtin_bswap32(value);
            return;
        default: break;
        }
    } else if (sci1_start <= address && address <= sci1_end) {
        if (n == 8) {
            sci1.write(address - sci1_start, value);
            return;
        }
    }

    fprintf(stderr, "Error: Invalid write access to 0x%06x\n", address);
}

template<uint8_t n, class T>
void update(uint32_t address, T(*f)(T))
{
    std::lock_guard<std::mutex> lock(mutex);

    if (ram_start <= address && address <= ram_end) {
        switch (n) {
        case  8:
            ram[address - ram_start] = f(ram[address- ram_start]);
            return;
        case 16: {
            uint16_t value = __builtin_bswap16(*(uint16_t*)&ram[address - ram_start]);
            *(uint16_t*)&ram[address - ram_start] = __builtin_bswap16(f(value));
            return;
        }
        case 32: {
            uint32_t value = __builtin_bswap32(*(uint32_t*)&ram[address - ram_start]);
            *(uint32_t*)&ram[address - ram_start] = __builtin_bswap32(value);
            return;
        }
        default: break;
        }
    } else if (sci1_start <= address && address <= sci1_end) {
        if (n == 8) {
            sci1.write(address - sci1_start, f(sci1.read(address - sci1_start)));
            return;
        }
    }

    fprintf(stderr, "Error: Invalid write access to 0x%06x\n", address);
}

    uint32_t load_elf(std::string filepath);
    uint32_t get_vector(uint8_t index);
    void dump(std::string filepath);

};

#endif
