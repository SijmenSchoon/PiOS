#include "mmio.hpp"


void mmio::write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

uint32_t mmio::read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}
