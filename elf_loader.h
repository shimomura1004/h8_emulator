#ifndef _ELF_LOADER_INCLUDED_
#define _ELF_LOADER_INCLUDED_

#include <string>
#include "inner_memory.h"

class ElfLoader {
public:
    static uint32_t load(InnerMemory* memory, std::string filepath);
};

#endif