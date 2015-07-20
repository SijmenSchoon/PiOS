#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

class mmio
{
public:
    static void write(uint32_t reg, uint32_t data);
    static uint32_t read(uint32_t reg);
};

#endif /* end of include guard: MMIO_H */
