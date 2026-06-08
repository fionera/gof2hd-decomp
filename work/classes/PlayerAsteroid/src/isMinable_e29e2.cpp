#include "class.h"

uint8_t PlayerAsteroid::isMinable()
{
    return F<uint8_t>(this, 0x13c);
}
