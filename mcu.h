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
    static const uint32_t ram_start = 0xffbf00; // 内蔵 RAM
    static const uint32_t ram_end   = 0xffff1f;
    static const uint32_t io_start  = 0xffff20; // 内部 I/O レジスタ
    static const uint32_t io_end    = 0xffffe9;

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
    {}

    template<uint8_t n, class T>
    T read(uint32_t address);

    template<uint8_t n, class T>
    void write(uint32_t address, T value);

    template<uint8_t n, class T>
    void update(uint32_t address, T(*f)(T));

    // todo: 実装する
    void dump(std::string filepath){}

};

#endif
