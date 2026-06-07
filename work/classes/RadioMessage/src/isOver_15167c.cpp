#include "class.h"

uint8_t RadioMessage::isOver()
{
    return field<uint8_t>(this, 0x21);
}
