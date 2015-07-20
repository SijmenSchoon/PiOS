#include "mailbox.hpp"
#include "mmio.hpp"

queue<uint32_t> mailbox::_queue[16];

static inline void memory_barrier()
{
    asm volatile("mcr p15, #0, %[zero], c7, c10, #5" : : [zero]"r"(0));
}

static inline void data_cache_flush()
{
    asm volatile("mcr p15, #0, %[zero], c7, c14, #0" : : [zero]"r"(0));
}

static inline void sync_barrier()
{
    asm volatile("mcr p15, #0, %[zero], c7, c10, #4" : : [zero]"r"(0));
}

void mailbox::write(uint32_t channel, uint32_t data)
{
    memory_barrier();
    data_cache_flush();

    // Wait for the mailbox to not be full
    while ((status() & MAIL_FULL) != 0);

    // Write the data
    *(unsigned int *)MAIL_SEND = (data << 4) | channel;

    memory_barrier();
}

uint32_t mailbox::read(uint32_t id)
{
    memory_barrier();
    data_cache_flush();

    volatile uint32_t msg;

    /*if (!_queue[id].empty())
    {
        // Check if there are messages waiting in the queue
        memory_barrier();
        return _queue[id].pop();
    }
    else
    {*/
        // Else, read them from the mailbox
        while (true)
        {
            // Wait for a message to come in
            while ((status() & MAIL_EMPTY) != 0);

            // Read the message
            msg = mmio::read(MAIL_RECV);
            if ((msg & 0x0F) != id)
            {
                _queue[msg & 0x0F].push((msg >> 4) & 0x0FFFFFFF);
            }
            else
            {
                break;
            }
        }

        memory_barrier();
        return (msg >> 4) & 0x0FFFFFFF;
    //}
}

uint32_t mailbox::status()
{
    memory_barrier();
    data_cache_flush();
    return mmio::read(MAIL_STATUS);
    memory_barrier();
}
