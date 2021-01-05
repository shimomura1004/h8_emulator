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

    void dump(std::string filepath);
};

#endif
