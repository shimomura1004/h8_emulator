#ifndef _DUMMY_MCU_INCLUDED_
#define _DUMMY_MCU_INCLUDED_

#include "mcu/mcu.h"
#include "dram/dram.h"

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

#endif
