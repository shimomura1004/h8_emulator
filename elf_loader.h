#ifndef _ELF_LOADER_INCLUDED_
#define _ELF_LOADER_INCLUDED_

#include <string>

class ElfLoader {
public:
    static uint32_t load(uint8_t* memory, std::string filepath);
};

#endif