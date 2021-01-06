#include <stdlib.h>
#include "inner_memory.h"
#include "elf_loader.h"

InnerMemory::InnerMemory()
{
    memory = (unsigned char*)malloc(sizeof(unsigned char) * 16 * 1024 * 1024);
}

InnerMemory::~InnerMemory()
{
    free(memory);
}

unsigned char& InnerMemory::operator[](unsigned long index)
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
    return *(uint32_t*)&memory[index * 4];
}

uint16_t InnerMemory::read_uint16(uint32_t address)
{
    return *(uint16_t*)&memory[address];
}

void InnerMemory::write_uint16(uint32_t address, uint16_t value)
{
    *(uint16_t*)&memory[address] = __builtin_bswap16(value);
}

uint32_t InnerMemory::read_uint32(uint32_t address)
{
    return *(uint32_t*)&memory[address];
}

void InnerMemory::write_uint32(uint32_t address, uint32_t value)
{
    *(uint32_t*)&memory[address] = __builtin_bswap32(value);
}

void InnerMemory::dump(std::string filepath)
{
    FILE* fp = fopen(filepath.c_str(), "wb");
    if (fp == nullptr) {
        return;
    }

    fwrite(memory, sizeof(unsigned char), 16 * 1024 * 1024, fp);

    fclose(fp);
}
