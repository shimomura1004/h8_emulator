#include <stdlib.h>
#include "inner_memory.h"

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

void InnerMemory::load_elf(std::string filepath)
{

}

unsigned long InnerMemory::get_vector(unsigned int index) const
{
    return *(unsigned long*)&memory[index * 4];
}
