#include "mcu.h"

template<uint8_t n, class T>
T MCU::read(uint32_t address)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (rom_start <= address && address <= rom_end) {
        switch (n) {
        case  8: return rom[address - rom_start];
        case 16: return __builtin_bswap16(*(uint16_t*)&rom[address - rom_start]);
        case 32: return __builtin_bswap32(*(uint32_t*)&rom[address - rom_start]);
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
}

template<uint8_t n, class T>
void MCU::write(uint32_t address, T value)
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
void MCU::update(uint32_t address, T(*f)(T))
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