#include "class.h"

uint8_t RadioMessage::isTriggered()
{
    return field<uint8_t>(this, 0x20);
}
