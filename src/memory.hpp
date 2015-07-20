#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

class memory
{
public:
    static uint32_t vtop(uint32_t virtualaddr);
    static uint32_t ptov(uint32_t physicaladdr);
};

#endif /* end of include guard: MEMORY_H */
