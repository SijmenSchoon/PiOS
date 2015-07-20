#include "framebuffer.hpp"
#include "mailbox.hpp"
#include "memory.hpp"

#define GPIO_BASE 0x3F200000UL

volatile unsigned int *gpio_set1 = (unsigned int *)(GPIO_BASE + 0x20);
volatile unsigned int *gpio_clr1 = (unsigned int *)(GPIO_BASE + 0x2C);

void fail(int speed = 20000)
{

    while (true)
    {
        *gpio_clr1 |= (1 << 15);
        for (volatile int i = 0; i < speed; i++);
        *gpio_set1 |= (1 << 15);
        for (volatile int i = 0; i < speed; i++);
    }
}

void blink()
{
    *gpio_clr1 |= (1 << 15);
    for (volatile int i = 0; i < 100000; i++);
    *gpio_set1 |= (1 << 15);
    for (volatile int i = 0; i < 100000; i++);
    *gpio_clr1 |= (1 << 15);
}

void framebuffer::init(int width, int height, int depth)
{
    if (width > 4096 || height > 4096 || depth > 32) return;

    volatile uint32_t mailbuffer[256] __attribute__((aligned(16)));
    mailbuffer[ 0] = sizeof(uint32_t) * 22;
    mailbuffer[ 1] = 0;
    mailbuffer[ 2] = mailbox::TAG_SET_PHYSICAL_DISPLAY;
    mailbuffer[ 3] = 8; // Value buffer size
    mailbuffer[ 4] = 8; // Request/response (0 = request)
    mailbuffer[ 5] = width;
    mailbuffer[ 6] = height;
    mailbuffer[ 7] = mailbox::TAG_SET_VIRTUAL_BUFFER;
    mailbuffer[ 8] = 0;
    mailbuffer[ 9] = 8;
    mailbuffer[10] = width;
    mailbuffer[11] = height;
    mailbuffer[12] = mailbox::TAG_SET_DEPTH;
    mailbuffer[13] = 4;
    mailbuffer[14] = 4;
    mailbuffer[15] = depth;
    mailbuffer[16] = mailbox::TAG_ALLOCATE_BUFFER;
    mailbuffer[17] = 8;
    mailbuffer[18] = 4;
    mailbuffer[19] = 16;
    mailbuffer[20] = 0;
    mailbuffer[21] = 0; // End tag

    blink();

    mailbox::write(8, (uint32_t)mailbuffer);

    blink();

    if (mailbuffer[1] == 0)
    {
        fail(50000);
    }
    else if (mailbuffer[1] == 0x80000001)
    {
        while (true);
    }

    uint8_t *pointer = (uint8_t *)mailbuffer[19];
    uint32_t size = mailbuffer[20];

    // Clear the framebuffer to white
    for (uint32_t i = 0; i < size; i++)
    {
        pointer[i] = 255;
    }
}
