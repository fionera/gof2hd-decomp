#include "class.h"

uint8_t Radar::isPlasmaInRange()
{
    return F<uint8_t>(this, 0x130);
}
