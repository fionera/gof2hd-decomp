#include "class.h"

void RadioMessage::finish()
{
    field<uint8_t>(this, 0x21) = 1;
}
