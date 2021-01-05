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

    const unsigned char& operator[](unsigned long index) const;
    void load_elf(std::string filepath);

    // 割込みベクタにアクセス
    unsigned long* vector(unsigned int index) const;

};

#endif
