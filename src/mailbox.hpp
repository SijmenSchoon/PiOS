#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>
#include "queue.hpp"

class mailbox
{
public:
    static void write(uint32_t channel, uint32_t data);
    static uint32_t read(uint32_t channel);
    static uint32_t status();

    enum
    {
        MAIL_RECV   = 0x3F00B880,
        MAIL_PEEK   = 0x3F00B890,
        MAIL_SEND   = 0x3F00B8A0,
        MAIL_STATUS = 0x3F00B898,

        MAIL_FULL   = 0x80000000,
        MAIL_EMPTY  = 0x40000000,

        TAG_ALLOCATE_BUFFER      = 0x00040001,
        // Set physical display width/height
        TAG_SET_PHYSICAL_DISPLAY = 0x00048003,
        // Set virtual buffer width/height
        TAG_SET_VIRTUAL_BUFFER   = 0x00048004,
        TAG_SET_DEPTH            = 0x00048005,
        TAG_SET_PIXEL_ORDER      = 0x00048006,
        TAG_SET_ALPHA_MODE       = 0x00048007,
        TAG_SET_VIRTUAL_OFFSET   = 0x00048009,
        TAG_SET_OVERSCAN         = 0x0004800A,
        TAG_SET_PALETTE          = 0x0004800B,
    };

private:
    static queue<uint32_t> _queue[16];
};

#endif /* end of include guard: MAILBOX_H */
