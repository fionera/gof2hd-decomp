#include "class.h"

void RadioMessage::trigger()
{
    field<uint8_t>(this, 0x20) = 1;
}
