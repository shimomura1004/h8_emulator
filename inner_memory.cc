#include <stdlib.h>
#include "inner_memory.h"
#include "elf_loader.h"

const uint32_t InnerMemory::memory_size       = 0x01000000;
const uint32_t InnerMemory::rom_start_address = 0x00000000;
const uint32_t InnerMemory::rom_end_address   = 0x00080000;

bool InnerMemory::check_if_valid_read_address(uint32_t address)
{
    return address < memory_size;
}

bool InnerMemory::check_if_valid_write_address(uint32_t address)
{
    bool write_to_rom = (rom_start_address <= address) && (address < rom_end_address);
    bool out_of_bounds = address >= memory_size;
    return !write_to_rom && !out_of_bounds;
}

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
    std::lock_guard<std::mutex> lock(mutex);
    return memory[index];
}

uint32_t InnerMemory::load_elf(std::string filepath)
{
    ElfLoader elf_loader;
    return elf_loader.load(this, filepath);
}

uint32_t InnerMemory::get_vector(unsigned int index)
{
    std::lock_guard<std::mutex> lock(mutex);
    return __builtin_bswap32(*(uint32_t*)&memory[index * 4]);
}

uint8_t InnerMemory::read_uint8(uint32_t address)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_read_address(address)) {
        return memory[address];
    } else {
        fprintf(stderr, "Error: Attempt to read invalid address: 0x%06x\n", address);
        return 0;
    }
}

void InnerMemory::write_uint8(uint32_t address, uint8_t value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_write_address(address)) {
        memory[address] = value;
    } else {
        fprintf(stderr, "Warning: Attempted to write to ROM(0x%08x). Ignored.\n", address);
    }
}

// todo: 16,32ビット用も作りたいが、エンディアンを隠蔽できなくなる
void InnerMemory::update_uint8(uint32_t address, uint8_t(*updater)(uint8_t))
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_write_address(address)) {
        memory[address] = updater(memory[address]);
    } else {
        fprintf(stderr, "Warning: Attempted to write to ROM(0x%08x). Ignored.\n", address);
    }
}

uint16_t InnerMemory::read_uint16(uint32_t address)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_read_address(address)) {
        return __builtin_bswap16(*(uint16_t*)&memory[address]);
    } else {
        fprintf(stderr, "Error: Attempt to read invalid address: 0x%06x\n", address);
        return 0;
    }
}

void InnerMemory::write_uint16(uint32_t address, uint16_t value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_write_address(address)) {
        *(uint16_t*)&memory[address] = __builtin_bswap16(value);
    } else {
        fprintf(stderr, "Warning: Attempted to write to ROM(0x%08x). Ignored.\n", address);
    }
}

uint32_t InnerMemory::read_uint32(uint32_t address)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_read_address(address)) {
        return __builtin_bswap32(*(uint32_t*)&memory[address]);
    } else {
        fprintf(stderr, "Error: Attempt to read invalid address: 0x%06x\n", address);
        return 0;
    }
}

void InnerMemory::write_uint32(uint32_t address, uint32_t value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (check_if_valid_write_address(address)) {
        *(uint32_t*)&memory[address] = __builtin_bswap32(value);
    } else {
        fprintf(stderr, "Warning: Attempted to write to ROM(0x%08x). Ignored.\n", address);
    }
}

void InnerMemory::dump(std::string filepath)
{
    std::lock_guard<std::mutex> lock(mutex);

    FILE* fp = fopen(filepath.c_str(), "wb");
    if (fp == nullptr) {
        return;
    }

    fwrite(memory, sizeof(uint8_t), memory_size, fp);

    fclose(fp);
}
