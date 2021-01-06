#ifndef _MEMORY_INCLUDED_
#define _MEMORY_INCLUDED_

#include <cstddef>
#include <string>

class InnerMemory {
private:
    unsigned char *memory;

public:
    InnerMemory();
    ~InnerMemory();

    unsigned char& operator[](unsigned long index);
    uint32_t load_elf(std::string filepath);
    uint32_t get_vector(unsigned int index) const;

    uint16_t read_uint16(uint32_t address);
    void write_uint16(uint32_t address, uint16_t value);
    uint32_t read_uint32(uint32_t address);
    void write_uint32(uint32_t address, uint32_t value);

    void dump(std::string filepath);

};

#endif
