#include <stdlib.h>
#include "inner_memory.h"
#include "elf_loader.h"

const uint32_t InnerMemory::memory_size       = 0x01000000;
const uint32_t InnerMemory::rom_start_address = 0x00000000;
const uint32_t InnerMemory::rom_end_address   = 0x00080000;

InnerMemory::InnerMemory()
{
    memory = (uint8_t*)malloc(sizeof(uint8_t) * memory_size);
}

InnerMemory::~InnerMemory()
{
    free(memory);
}

uint8_t& InnerMemory::operator[](unsigned long index)
{
    return memory[index];
}

uint32_t InnerMemory::load_elf(std::string filepath)
{
    ElfLoader elf_loader;
    return elf_loader.load(this, filepath);
}

uint32_t InnerMemory::get_vector(unsigned int index) const
{
    return __builtin_bswap32(*(uint32_t*)&memory[index * 4]);
}

uint8_t InnerMemory::read_uint8(uint32_t address)
{
    return memory[address];
}

void InnerMemory::write_uint8(uint32_t address, uint8_t value)
{
    if ((rom_start_address <= address) && (address < rom_end_address)) {
        fprintf(stderr, "Warning: attempted to write to ROM(0x%08x). Ignored.\n", address);
    } else {
        memory[address] = value;
    }
}

uint16_t InnerMemory::read_uint16(uint32_t address)
{
    return __builtin_bswap16(*(uint16_t*)&memory[address]);
}

void InnerMemory::write_uint16(uint32_t address, uint16_t value)
{
    if ((rom_start_address <= address) && (address < rom_end_address)) {
        fprintf(stderr, "Warning: attempted to write to ROM(0x%08x). Ignored.\n", address);
    } else {
        *(uint16_t*)&memory[address] = __builtin_bswap16(value);
    }
}

uint32_t InnerMemory::read_uint32(uint32_t address)
{
    return __builtin_bswap32(*(uint32_t*)&memory[address]);
}

void InnerMemory::write_uint32(uint32_t address, uint32_t value)
{
    if ((rom_start_address <= address) && (address < rom_end_address)) {
        fprintf(stderr, "Warning: attempted to write to ROM(0x%08x). Ignored.\n", address);
    } else {
        *(uint32_t*)&memory[address] = __builtin_bswap32(value);
    }
}

void InnerMemory::dump(std::string filepath)
{
    FILE* fp = fopen(filepath.c_str(), "wb");
    if (fp == nullptr) {
        return;
    }

    fwrite(memory, sizeof(uint8_t), memory_size, fp);

    fclose(fp);
}
