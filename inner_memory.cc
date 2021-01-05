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
    return *(unsigned long*)&memory[index * 4];
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
