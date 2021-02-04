#ifndef _MCU_INCLUDED_
#define _MCU_INCLUDED_

#include <cstdint>
#include <string>
#include <mutex>
#include "sci/sci.h"
#include "timer8.h"

class MCU {
    // モード5(内蔵ROM有効拡張16Mバイトモード) EMCビットが1(初期値)のとき
    static const uint32_t vec_start = 0x000000; // ベクタエリア
    static const uint32_t vec_end   = 0x0000ff;
    static const uint32_t rom_start = 0x000100; // 内蔵 ROM
    static const uint32_t rom_end   = 0x07ffff;
                                                // 外部アドレス空間
                                                // 内部 I/O レジスタ
    static const uint32_t ram_start = 0xffbf00; // 内蔵 RAM
    static const uint32_t ram_end   = 0xffff1f;
    static const uint32_t io_start  = 0xffff20; // 内部 I/O レジスタ

    // 内蔵機能のレジスタのアドレスは8ビットアドレス空間に配置されている
    // 短い命令調で指定できるため便利
    // SCI やタイマなどはここに入っているため、モードによってアドレスが異なる
    // ノーマルモードだとタイマは 0xfff80、アドバンストモードだと 0xffff80
    static const uint32_t timer01_start = 0xffff80; // タイマ0,1
    static const uint32_t timer01_end   = 0xffff89;
    static const uint32_t timer23_start = 0xffff90; // タイマ2,3
    static const uint32_t timer23_end   = 0xffff99;

    static const uint32_t sci0_start = 0xffffb0; // SCI0
    static const uint32_t sci0_end   = 0xffffb6;
    static const uint32_t sci1_start = 0xffffb8; // SCI1
    static const uint32_t sci1_end   = 0xffffbe;
    static const uint32_t sci2_start = 0xffffc0; // SCI2
    static const uint32_t sci2_end   = 0xffffc6;

    static const uint32_t io_end    = 0xffffe9;
                                                // 外部アドレス空間
    static const uint32_t all_end   = 0xffffff;


    // ROM/RAM の実体
    uint8_t rom[rom_end - rom_start + 1];
    uint8_t ram[ram_end - ram_start + 1];

    // H8/3069F には3つの SCI が内蔵されている
    SCI** sci;

    // 8ビットタイマ
    Timer8 *timer8_01;

    // メモリへの読み書きが競合しないようにする
    std::mutex mutex;

public:
    MCU(SCI** sci, Timer8 *timer8_01, std::mutex& mutex);

    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);

    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);

    uint32_t load_elf(std::string filepath);
    uint32_t get_vector(uint8_t index);
    void dump(std::string filepath);

};

#endif
