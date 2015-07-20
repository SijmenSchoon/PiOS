#include <stdbool.h>
#include <stdint.h>
//#include "framebuffer.hpp"

#define GPIO_BASE 0x3F200000UL

volatile unsigned int *gpio_fs0  = (unsigned int *)(GPIO_BASE + 0x00);
volatile unsigned int *gpio_fs1  = (unsigned int *)(GPIO_BASE + 0x04);
volatile unsigned int *gpio_fs2  = (unsigned int *)(GPIO_BASE + 0x08);
volatile unsigned int *gpio_fs3  = (unsigned int *)(GPIO_BASE + 0x0c);
volatile unsigned int *gpio_fs4  = (unsigned int *)(GPIO_BASE + 0x10);
volatile unsigned int *gpio_fs5  = (unsigned int *)(GPIO_BASE + 0x14);

volatile unsigned int *gpio_set0 = (unsigned int *)(GPIO_BASE + 0x1c);
volatile unsigned int *gpio_set1 = (unsigned int *)(GPIO_BASE + 0x20);

volatile unsigned int *gpio_clr0 = (unsigned int *)(GPIO_BASE + 0x28);
volatile unsigned int *gpio_clr1 = (unsigned int *)(GPIO_BASE + 0x2c);

void pin_mode(unsigned int pin, unsigned int mode)
{
    if (pin < 10)
    {
        *gpio_fs0 &= ~(0b111 << (pin * 3));
        *gpio_fs0 |= (mode << (pin * 3));
    }
    else if (pin < 20)
    {
        *gpio_fs1 &= ~(0b111 << ((pin - 10) * 3));
        *gpio_fs1 |= mode << ((pin - 10) * 3);
    }
    else if (pin < 30)
    {
        *gpio_fs2 &= ~(0b111 << ((pin - 20) * 3));
        *gpio_fs2 |= mode << ((pin - 20) * 3);
    }
    else if (pin < 40)
    {
        *gpio_fs3 &= ~(0b111 << ((pin - 30) * 3));
        *gpio_fs3 |= mode << ((pin - 30) * 3);
    }
    else if (pin < 50)
    {
        *gpio_fs4 &= ~(0b111 << ((pin - 40) * 3));
        *gpio_fs4 |= mode << ((pin - 40) * 3);
    }
    else if (pin < 54)
    {
        *gpio_fs5 &= ~(0b111 << ((pin - 50) * 3));
        *gpio_fs5 |= mode << ((pin - 50) * 3);
    }
}

void digital_write(int pin, bool state)
{
    if (state)
    {
        // high
        if (pin < 32)
        {
            *gpio_set0 |= (1 << pin);
        }
        else
        {
            *gpio_set1 |= (1 << pin);
        }
    }
    else
    {
        // low
        if (pin < 32)
        {
            *gpio_clr0 |= (1 << pin);
        }
        else
        {
            *gpio_clr1 |= (1 << pin);
        }
    }
}

void delay(int ms)
{
    for (volatile int i = 0; i < ms * 914; i++);
}

const int PIN_D4 = 5, PIN_D5 = 4, PIN_D6 = 3, PIN_D7 = 2;
const int PIN_E = 8, PIN_RS = 7;

void lcd_command(bool rs, uint8_t data)
{
    // First nibble
    digital_write(PIN_E, 1);
    delay(10);
    delay(10);
    digital_write(PIN_RS, rs);
    delay(10);
    delay(10);
    digital_write(PIN_D7, (data >> 7) & 1);
    delay(10);
    digital_write(PIN_D6, (data >> 6) & 1);
    delay(10);
    digital_write(PIN_D5, (data >> 5) & 1);
    delay(10);
    digital_write(PIN_D4, (data >> 4) & 1);
    delay(10);
    delay(10);
    digital_write(PIN_E, 0);
    delay(10);
    delay(10);

    // Second nibble
    digital_write(PIN_E, 1);
    delay(10);
    delay(10);
    digital_write(PIN_D7, (data >> 3) & 1);
    delay(10);
    digital_write(PIN_D6, (data >> 2) & 1);
    delay(10);
    digital_write(PIN_D5, (data >> 1) & 1);
    delay(10);
    digital_write(PIN_D4, (data >> 0) & 1);
    delay(10);
    delay(10);
    digital_write(PIN_E, 0);
    delay(10);
    delay(10);
}

int kmain()
{
    pin_mode(35, 1);
    digital_write(35, 1);
    delay(250);
    digital_write(35, 0);
    delay(250);
    digital_write(35, 1);

    pin_mode(PIN_D4, 1);
    pin_mode(PIN_D5, 1);
    pin_mode(PIN_D6, 1);
    pin_mode(PIN_D7, 1);
    pin_mode(PIN_E,  1);
    pin_mode(PIN_RS, 1);

    // 4 bit mode
    lcd_command(0, 0b00100000);

    // function set, 4 bit, 2 lines, 5x7
    lcd_command(0, 0b00101000);

    // display on, cursor on, cursor blinking
    lcd_command(0, 0b00001111);

    // entry mode, increment, no shift
    lcd_command(0, 0b00000110);

    // clear
    lcd_command(0, 0b00000001);

    lcd_command(1, 'H');
    lcd_command(1, 'e');
    lcd_command(1, 'l');
    lcd_command(1, 'l');
    lcd_command(1, 'o');
    lcd_command(1, ' ');
    lcd_command(1, 'w');
    lcd_command(1, 'o');
    lcd_command(1, 'r');
    lcd_command(1, 'l');
    lcd_command(1, 'd');
    lcd_command(1, '!');

    while (true);

    return 0;
}
