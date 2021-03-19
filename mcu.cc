#include "mcu.h"
#include "elf_loader.h"

// todo: ヘルパ関数としてどこかに定義

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define bswap16_if_little_endian(v) (__builtin_bswap16(v));
#define bswap32_if_little_endian(v) (__builtin_bswap32(v));

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define bswap16_if_little_endian(v) (v)
#define bswap32_if_little_endian(v) (v)

#endif
#endif

MCU::MCU(SCI** sci, Timer8* timer8, IOPort* ioport)
    : sci(sci)
    , timer8(timer8)
    , ioport(ioport)
{}

uint8_t MCU::read8(uint32_t address)
{
    if (vec_start <= address && address <= rom_end) {
        // ROM は更新されないのでロック不要
        return rom[address];
    } else if (ram_start <= address && address <= ram_end) {
        // RAM は更新されうるのでロック
        std::lock_guard<std::mutex> lock(mutex);
        return ram[address - ram_start];
    } else if (timer01_start <= address && address <= timer01_end) {
        return timer8->read8(address - timer01_start, 0);
    } else if (timer23_start <= address && address <= timer23_end) {
        return timer8->read8(address - timer23_start, 1);
    } else if (ioport_ddr_start <= address && address <= ioport_ddr_end) {
        return ioport->read8(address - ioport_ddr_start);
    } else if (area1_start <= address && address <= area1_end) {
        return rtl8019as.read8(address - area1_start);
    } else if (area2_start <= address && address <= area2_end) {
        return dram2.read8(address - area2_start);
    } else if (sci0_start <= address && address <= sci0_end) {
        // SCI のロックは SCI 側で実施
        return sci[0]->read(address - sci0_start);
    } else if (sci1_start <= address && address <= sci1_end) {
        return sci[1]->read(address - sci1_start);
    } else if (sci2_start <= address && address <= sci2_end) {
        return sci[2]->read(address - sci2_start);
    } else if (BusController::brcr_address <= address && address <= BusController::rtcor_address) {
        return bus_controller.read(address);
    } else {
        fprintf(stderr, "Error: Invalid read(8) access to 0x%06x\n", address);
        return 0;
    }
}

uint16_t MCU::read16(uint32_t address)
{
    if (vec_start <= address && address <= rom_end) {
        return bswap16_if_little_endian(*(uint16_t*)&rom[address]);
    } else if (ram_start <= address && address <= ram_end) {
        std::lock_guard<std::mutex> lock(mutex);
        return bswap16_if_little_endian(*(uint16_t*)&ram[address - ram_start]);
    } else if (timer01_start <= address && address <= timer01_end) {
        return this->timer8->read16(address - timer01_start, 0);
    } else if (timer23_start <= address && address <= timer23_end) {
        return this->timer8->read16(address - timer23_start, 1);
    } else if (area2_start <= address && address <= area2_end) {
        return dram2.read16(address - area2_start);
    } else {
        fprintf(stderr, "Error: Invalid read(16) access to 0x%06x\n", address);
        return 0;
    }
}

uint32_t MCU::read32(uint32_t address)
{
    if (vec_start <= address && address <= rom_end) {
        return bswap32_if_little_endian(*(uint32_t*)&rom[address]);
    } else if (ram_start <= address && address <= ram_end) {
        std::lock_guard<std::mutex> lock(mutex);
        return bswap32_if_little_endian(*(uint32_t*)&ram[address - ram_start]);
    } else if (area2_start <= address && address <= area2_end) {
        return dram2.read32(address - area2_start);
    } else {
        fprintf(stderr, "Error: Invalid read(32) access to 0x%06x\n", address);
        return 0;
    }
}

void MCU::write8(uint32_t address, uint8_t value)
{
    if (ram_start <= address && address <= ram_end) {
        std::lock_guard<std::mutex> lock(mutex);
        ram[address - ram_start] = value;
    } else if (timer01_start <= address && address <= timer01_end) {
        timer8->write8(address - timer01_start, value, 0);
    } else if (timer23_start <= address && address <= timer23_end) {
        timer8->write8(address - timer23_start, value, 1);
    } else if (ioport_ddr_start <= address && address <= ioport_ddr_end) {
        ioport->write8(address - ioport_ddr_start, value);
    } else if (area1_start <= address && address <= area1_end) {
        rtl8019as.write8(address - area1_start, value);
    } else if (area2_start <= address && address <= area2_end) {
        dram2.write8(address - area2_start, value);
    } else if (sci0_start <= address && address <= sci0_end) {
        // SCI のロックは SCI 側で実施
        sci[0]->write(address - sci0_start, value);
    } else if (sci1_start <= address && address <= sci1_end) {
        sci[1]->write(address - sci1_start, value);
    } else if (sci2_start <= address && address <= sci2_end) {
        sci[2]->write(address - sci2_start, value);
    } else if (BusController::brcr_address <= address && address <= BusController::rtcor_address) { 
        bus_controller.write(address, value);
    } else {
        fprintf(stderr, "Error: Invalid write(8) access to 0x%06x\n", address);
    }
}

void MCU::write16(uint32_t address, uint16_t value)
{
    if (ram_start <= address && address <= ram_end) {
        *(uint16_t*)&ram[address - ram_start] = bswap16_if_little_endian(value);
    } else if (timer01_start <= address && address <= timer01_end) {
        timer8->write16(address - timer01_start, value, 0);
    } else if (timer23_start <= address && address <= timer23_end) {
        timer8->write16(address - timer23_start, value, 1);
    } else if (area2_start <= address && address <= area2_end) {
        dram2.write16(address - area2_start, value);
    } else {
        fprintf(stderr, "Error: Invalid write(16) access to 0x%06x\n", address);
    }
}

void MCU::write32(uint32_t address, uint32_t value)
{
    if (ram_start <= address && address <= ram_end) {
        *(uint32_t*)&ram[address - ram_start] = bswap32_if_little_endian(value);
    } else if (area2_start <= address && address <= area2_end) {
        dram2.write32(address - area2_start, value);
    } else {
        fprintf(stderr, "Error: Invalid write(32) access to 0x%06x\n", address);
    }
}

uint32_t MCU::load_elf(std::string filepath)
{
    return ElfLoader::load(rom, filepath);
}

uint32_t MCU::get_vector(uint8_t index)
{
    return read32(index * 4);
}

void MCU::dump(std::string filepath)
{
    std::lock_guard<std::mutex> lock(mutex);

    FILE* fp = fopen(filepath.c_str(), "wb");
    if (fp == nullptr) {
        return;
    }

    // 内蔵ROMを出力
    fwrite(rom, sizeof(uint8_t), rom_end - vec_start + 1, fp);

    // 内蔵 ROM から DRAM(エリア2)までは0で埋める
    for (uint32_t i = rom_end + 1; i < area2_start; i++) {
        fputc(0, fp);
    }

    // todo: RTL8019AS のダンプに対応

    dram2.dump(fp);

    // DRAM(エリア2)から内蔵 RAM までは0で埋める
    for (uint32_t i = area2_end + 1; i < ram_start; i++) {
        fputc(0, fp);
    }

    // 内蔵RAMを出力
    fwrite(ram, sizeof(uint8_t), ram_end - ram_start + 1, fp);
    // 内部I/Oレジスタ(SCI を含む)
    // SCI1 以外は 0 埋めする
    for (uint32_t i = ram_end + 1; i < sci0_start; i++) {
        fputc(0, fp);
    }

    // todo: タイマのダンプに対応
    // todo: IO ポートのダンプに対応

    sci[0]->dump(fp);
    sci[1]->dump(fp);
    sci[2]->dump(fp);

    // 末尾まで0 埋めする
    for (uint32_t i=sci2_end + 1; i < all_end + 1; i++) {
        fputc(0, fp);
    }

    fclose(fp);
}
