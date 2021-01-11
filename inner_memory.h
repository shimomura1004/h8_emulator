#ifndef _MEMORY_INCLUDED_
#define _MEMORY_INCLUDED_

#include <cstddef>
#include <string>
#include <mutex>

class InnerMemory {
    const static uint32_t memory_size;
    const static uint32_t rom_start_address;
    const static uint32_t rom_end_address;

private:
    uint8_t *memory;
    std::mutex mutex;

public:
    InnerMemory();
    ~InnerMemory();

    uint8_t& operator[](unsigned long index);
    uint32_t load_elf(std::string filepath);
    uint32_t get_vector(unsigned int index);

    uint8_t read_uint8(uint32_t address);
    void write_uint8(uint32_t address, uint8_t value);
    uint16_t read_uint16(uint32_t address);
    void write_uint16(uint32_t address, uint16_t value);
    uint32_t read_uint32(uint32_t address);
    void write_uint32(uint32_t address, uint32_t value);

    void dump(std::string filepath);

};

#endif
