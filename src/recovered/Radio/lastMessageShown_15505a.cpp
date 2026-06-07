#include "class.h"

uint8_t Radio::lastMessageShown()
{
    return F<uint8_t>(this, 0x2c);
}
