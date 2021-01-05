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
    void load_elf(std::string filepath);
    unsigned long get_vector(unsigned int index) const;

};

#endif
