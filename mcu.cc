#include "mcu.h"
#include "elf_loader.h"

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define bswap16_if_little_endian(v) (__builtin_bswap16(v));
#define bswap32_if_little_endian(v) (__builtin_bswap32(v));

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define bswap16_if_little_endian(v) (v)
#define bswap32_if_little_endian(v) (v)

#endif
#endif

MCU::MCU(InterruptController& interrupt_controller, std::mutex& mutex)
    : sci0(0, interrupt_controller, mutex)
    , sci1(1, interrupt_controller, mutex)
    , sci2(2, interrupt_controller, mutex)
{
    sci0.run();
    sci1.run();
    sci2.run();
}

uint8_t MCU::read8(uint32_t address)
{
    if (vec_start <= address && address <= rom_end) {
        // ROM は更新されないのでロック不要
        return rom[address];
    } else if (ram_start <= address && address <= ram_end) {
        // RAM は更新されうるのでロック
        std::lock_guard<std::mutex> lock(mutex);
        return ram[address - ram_start];
    } else if (sci0_start <= address && address <= sci0_end) {
        // SCI のロックは SCI 側で実施
        return sci0.read(address - sci0_start);
    } else if (sci1_start <= address && address <= sci1_end) {
        return sci1.read(address - sci1_start);
    } else if (sci2_start <= address && address <= sci2_end) {
        return sci2.read(address - sci2_start);
    } else {
        fprintf(stderr, "Error: Invalid read access to 0x%06x\n", address);
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
    } else {
        fprintf(stderr, "Error: Invalid read access to 0x%06x\n", address);
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
    } else {
        fprintf(stderr, "Error: Invalid read access to 0x%06x\n", address);
        return 0;
    }
}

void MCU::write8(uint32_t address, uint8_t value)
{
    if (ram_start <= address && address <= ram_end) {
        std::lock_guard<std::mutex> lock(mutex);
        ram[address - ram_start] = value;
    } else if (sci0_start <= address && address <= sci0_end) {
        // SCI のロックは SCI 側で実施
        sci0.write(address - sci0_start, value);
    } else if (sci1_start <= address && address <= sci1_end) {
        sci1.write(address - sci1_start, value);
    } else if (sci2_start <= address && address <= sci2_end) {
        sci2.write(address - sci2_start, value);
    } else {
        fprintf(stderr, "Error: Invalid write access to 0x%06x\n", address);
    }
}

void MCU::write16(uint32_t address, uint16_t value)
{
    if (ram_start <= address && address <= ram_end) {
        *(uint16_t*)&ram[address - ram_start] = bswap16_if_little_endian(value);
    } else {
        fprintf(stderr, "Error: Invalid write access to 0x%06x\n", address);
    }
}

void MCU::write32(uint32_t address, uint32_t value)
{
    if (ram_start <= address && address <= ram_end) {
        *(uint32_t*)&ram[address - ram_start] = bswap32_if_little_endian(value);
    } else {
        fprintf(stderr, "Error: Invalid write access to 0x%06x\n", address);
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
    // 内蔵RAMまでは0で埋める(外部アドレス空間 + 内部I/Oレジスタ)
    for (uint32_t i = rom_end + 1; i < ram_start; i++) {
        fputc(0, fp);
    }
    // 内蔵RAMを出力
    fwrite(ram, sizeof(uint8_t), ram_end - ram_start + 1, fp);
    // 内部I/Oレジスタ(SCI を含む)
    // SCI1 以外は 0 埋めする
    for (uint32_t i = ram_end + 1; i < sci0_start; i++) {
        fputc(0, fp);
    }

    sci0.dump(fp);
    sci1.dump(fp);
    sci2.dump(fp);

    // 末尾まで0 埋めする
    for (uint32_t i=sci2_end + 1; i < all_end + 1; i++) {
        fputc(0, fp);
    }

    fclose(fp);
}
