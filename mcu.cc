#include "mcu.h"
#include "elf_loader.h"

uint32_t MCU::load_elf(std::string filepath)
{
    return ElfLoader::load(rom, filepath);
}

uint32_t MCU::get_vector(uint8_t index)
{
    return read<32, uint32_t>(index * 4);
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
    for (uint32_t i = ram_end + 1; i < sci1_start; i++) {
        fputc(0, fp);
    }
    // SCI1 を出力
    sci1.dump(fp);
    // 末尾まで0 埋めする
    for (uint32_t i=sci2_start; i < all_end + 1; i++) {
        fputc(0, fp);
    }

    fclose(fp);
}
