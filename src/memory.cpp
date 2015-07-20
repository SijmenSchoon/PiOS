#include "memory.hpp"

uint32_t memory::vtop(uint32_t virtualaddr)
{
    uint32_t pt_data = ((uint32_t *)ptov(0x4000))[virtualaddr >> 20];
    switch (pt_data & 3)
    {
        case 0:
        case 3:
            // Nothing mapped
            return 0xffffffff;

        case 2:
            return (pt_data & 0xfff00000) +
                (virtualaddr & ((pt_data & (1<<18)) ? 0x00ffffff : 0x000fffff));
    }

    uint32_t cpt_data = ((uint32_t *)(0x80000000 + (pt_data & 0xfffffc00)))[(virtualaddr >> 12) & 0xff];

    if ((cpt_data & 3) == 0)
    {
        // Nothing mapped
        return 0xffffffff;
    }
    if (cpt_data & 2)
    {
        // Small (4k) page
        return (cpt_data & 0xfffff000) + (virtualaddr & 0x0fff);
    }

    // Large (64k) page
    return (cpt_data & 0xffff0000) + (virtualaddr & 0xffff);
}

uint32_t memory::ptov(uint32_t physicaladdr)
{
    return physicaladdr + 0x80000000;
}
