#include <gtest/gtest.h>
#include "instructions/mov_immediate.h"

#include "cpu/h8300h_cpu.h"
#include "dram/h8300h_dram.h"
#include "sci/h8300h_sci.h"
#include "timer/h8300h_timer8.h"
#include "net/rtl8019as.h"
#include "interrupt/general_interrupt_controller.h"
#include "mcu.h"

// todo: ダミークラスは他のテストと共通化する
class DummyMCU : public IMCU {
public:     
    static const uint32_t vec_start = 0x000000; // ベクタエリア
    static const uint32_t vec_end   = 0x0000ff;
    static const uint32_t rom_start = 0x000100; // 内蔵 ROM
    static const uint32_t rom_end   = 0x07ffff;
    static const uint32_t ram_start = 0xffbf00; // 内蔵 RAM
    static const uint32_t ram_end   = 0xffff1f;
    static const uint32_t area2_start = 0x400000; // DRAM
    static const uint32_t area2_end   = 0x5fffff;

    IDRAM& dram;
    uint8_t rom[rom_end - rom_start + 1];
    uint8_t ram[ram_end - ram_start + 1];

public:
    DummyMCU(IDRAM& dram) : dram(dram) {}

    void init() override {}

    uint8_t read8(uint32_t address) override {
        if (vec_start <= address && address <= rom_end) {
            return rom[address];
        } else if (ram_start <= address && address <= ram_end) {
            return ram[address - ram_start];
        } else if (area2_start <= address && address <= area2_end) {
            return dram.read8(address - area2_start);
        } else {
            fprintf(stderr, "Error: Invalid read(8) access to 0x%06x\n", address);
            return 0;
        }
    }
    uint16_t read16(uint32_t address) override {
        if (vec_start <= address && address <= rom_end) {
            return bswap16_if_little_endian(*(uint16_t*)&rom[address]);
        } else if (ram_start <= address && address <= ram_end) {
            return bswap16_if_little_endian(*(uint16_t*)&ram[address - ram_start]);
        } else if (area2_start <= address && address <= area2_end) {
            return dram.read16(address - area2_start);
        } else {
            fprintf(stderr, "Error: Invalid read(16) access to 0x%06x\n", address);
            return 0;
        }
    }

    uint32_t read32(uint32_t address) override {
        if (vec_start <= address && address <= rom_end) {
            return bswap32_if_little_endian(*(uint32_t*)&rom[address]);
        } else if (ram_start <= address && address <= ram_end) {
            return bswap32_if_little_endian(*(uint32_t*)&ram[address - ram_start]);
        } else if (area2_start <= address && address <= area2_end) {
            return dram.read32(address - area2_start);
        } else {
            fprintf(stderr, "Error: Invalid read(32) access to 0x%06x\n", address);
            return 0;
        }
    }

    void write8(uint32_t address, uint8_t value) override {
        if (ram_start <= address && address <= ram_end) {
            ram[address - ram_start] = value;
        } else if (area2_start <= address && address <= area2_end) {
            dram.write8(address - area2_start, value);
        } else {
            fprintf(stderr, "Error: Invalid write(8) access to 0x%06x\n", address);
        }
    }
    void write16(uint32_t address, uint16_t value) override {
        if (ram_start <= address && address <= ram_end) {
            *(uint16_t*)&ram[address - ram_start] = bswap16_if_little_endian(value);
        } else if (area2_start <= address && address <= area2_end) {
            dram.write16(address - area2_start, value);
        } else {
            fprintf(stderr, "Error: Invalid write(16) access to 0x%06x\n", address);
        }
    }
    void write32(uint32_t address, uint32_t value) override {
        if (ram_start <= address && address <= ram_end) {
            *(uint32_t*)&ram[address - ram_start] = bswap32_if_little_endian(value);
        } else if (area2_start <= address && address <= area2_end) {
            dram.write32(address - area2_start, value);
        } else {
            fprintf(stderr, "Error: Invalid write(32) access to 0x%06x\n", address);
        }
    }

    uint32_t load_elf(std::string filepath) override { return 0; }
    uint32_t get_vector(uint8_t index) override { return this->read32(index * 4); }
    void dump(std::string filepath) override {}

};

// class DummySCI : public ISCI {
//     void run() override {}
//     void terminate() override {}

//     interrupt_t getInterrupt() override { return interrupt_t::NONE; }
//     void clearInterrupt(interrupt_t type) override {}

//     void dump(FILE* fp) override {}

// public:
//     uint8_t read(uint32_t address) override { return 0; }
//     void write(uint32_t address, uint8_t value) override {}

// };

// class DummyTimer : public ITimer8 {
//     interrupt_t getInterrupt() override { return interrupt_t::NONE; }
//     void clearInterrupt(interrupt_t type) override {}

//     uint8_t read8(uint32_t address, uint8_t channel) override { return 0; }
//     uint16_t read16(uint32_t address, uint8_t channel) override { return 0; }
//     void write8(uint32_t address, uint8_t value, uint8_t channel) override {}
//     void write16(uint32_t address, uint16_t value, uint8_t channel) override {}
// };

// class DummyNIC : public INIC {
// public:
//     void run() override {}
//     void terminate() override {}

//     interrupt_t getInterrupt() override { return interrupt_t::NONE; }
//     void clearInterrupt(interrupt_t type) override {}

//     uint8_t dma_read(uint16_t address) override { return 0; }
//     void dma_write(uint16_t address, uint8_t value) override {}

//     void dump(FILE* fp) override {}

// public:
//     uint8_t read8(uint32_t address) override { return 0; }
//     void write8(uint32_t address, uint8_t value) override {}

// };

class DummyInterruptController : public IInterruptController {
public:
    void set(interrupt_t type) override {}
    void clear(interrupt_t type) override {}

    interrupt_t getInterruptType() override { return interrupt_t::NONE; }
    interrupt_t getTrap() override { return interrupt_t::NONE; }

};

// todo: このフィクスチャは他のテストでも使える
// todo: CPU のテストに特有のものはこのフィクスチャから派生させて使う
class CPUTestFix : public ::testing::Test
{
protected:
     H8Board *h8;
     ICPU *cpu;
     IMCU *mcu;
     IDRAM *dram;
     IInterruptController *interrupt_controller;

     void SetUp() override
     {
          this->cpu = new H8300H_CPU();
          this->dram = new H8300H_DRAM();
          this->interrupt_controller = new DummyInterruptController;
          this->mcu = new DummyMCU(*dram);
          this->h8 = new H8Board(*this->cpu, *this->mcu, *this->interrupt_controller);
          this->h8->init();
    }

    void TearDown() override
    {
         delete this->h8;
         delete this->mcu;
         delete this->interrupt_controller;
         delete this->dram;
         delete this->cpu;
    }
};

// 命令の実行のテストと、命令のパースのテストは違う
// ここで確認するのはパーサの選択やパースの実行結果ではなく、命令の実行結果
TEST_F(CPUTestFix, mov_immediate)
{
     this->h8->cpu.pc() = DummyMCU::area2_start;

     // mov.b #0x12:8,r0l
     this->dram->write8(0, 0xf0);
     this->dram->write8(1, 0x12);

     // 正しく命令が実行され、8ビットレジスタ R0L に即値 0x12 がコピーされる
     int ret = this->h8->execute_next_instruction();
     EXPECT_EQ(ret, 0);
     int val = this->cpu->reg8(0).get();
     EXPECT_EQ(val, 0x12);
}

TEST_F(CPUTestFix, Test2)
{
}

// TEST(immediate_b_parse, normal)
// {
//      EXPECT_EQ(1, 1);

//      // テスト用のメモリ状態を返すクラスを作ってテスト
//      // parser/runner が想定どおりのものが返ってくるか、Instruction は正しく更新されるか
//      // instruction の全てのテストで使えるので、共通化する
// }
