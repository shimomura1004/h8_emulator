#ifndef _MCU_INCLUDED_
#define _MCU_INCLUDED_

#include <cstdint>
#include <string>
#include <mutex>
#include "sci/sci.h"
#include "timer/timer8.h"
#include "ioport/ioport.h"
#include "dram/dram.h"
#include "bus/buscontroller.h"
#include "net/rtl8019as.h"

class MCU {
    // モード5(内蔵ROM有効拡張16Mバイトモード) EMCビットが1(初期値)のとき
    static const uint32_t vec_start = 0x000000; // ベクタエリア
    static const uint32_t vec_end   = 0x0000ff;
    static const uint32_t rom_start = 0x000100; // 内蔵 ROM
    static const uint32_t rom_end   = 0x07ffff;
                                                // 外部アドレス空間

    static const uint32_t ioport_ddr_start = 0xfee000; // IOポート(0~B)
    static const uint32_t ioport_ddr_end   = 0xfee00a;

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

    // 外部アドレス空間
    static const uint32_t area0_start = 0x000000;
    static const uint32_t area0_end   = 0x1fffff;
    static const uint32_t area1_start = 0x200000; // RTL8019AS
    static const uint32_t area1_end   = 0x3fffff;
    static const uint32_t area2_start = 0x400000; // DRAM
    static const uint32_t area2_end   = 0x5fffff;
    static const uint32_t area3_start = 0x600000;
    static const uint32_t area3_end   = 0x7fffff;
    static const uint32_t area4_start = 0x800000;
    static const uint32_t area4_end   = 0x9fffff;
    static const uint32_t area5_start = 0xa00000;
    static const uint32_t area5_end   = 0xbfffff;
    static const uint32_t area6_start = 0xc00000;
    static const uint32_t area6_end   = 0xdfffff;
    static const uint32_t area7_start = 0xe00000;
    static const uint32_t area7_end   = 0xffffff;

    // todo: rom/ram は DRAM と共通化できる
    // 内蔵 ROM/RAM の実体
    uint8_t rom[rom_end - rom_start + 1];
    uint8_t ram[ram_end - ram_start + 1];

    // 外部 DRAM
    IDRAM *dram;

    // H8/3069F には3つの SCI が内蔵されている
    ISCI** sci;

    // 8ビットタイマ
    ITimer8 *timer8;

    // IO ポート
    IOPort *ioport;

    // RTL8019AS
    RTL8019AS *rtl8019as;

    // バスコントローラ
    BusController bus_controller;

    // メモリへの読み書きが競合しないようにする
    std::mutex mutex;

public:
    MCU(IDRAM *dram, ISCI** sci, ITimer8 *timer8, IOPort *ioport, RTL8019AS *rtl8019as);

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
