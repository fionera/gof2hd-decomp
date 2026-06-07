#include "class.h"

void RadioMessage::reset()
{
    volatile int *timer = &field<int>(this, 0x24);
    volatile uint16_t *flags = &field<uint16_t>(this, 0x20);
    *timer = 0;
    *flags = 0;
}
