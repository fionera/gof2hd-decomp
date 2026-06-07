#include "class.h"

uint8_t StarMap::missionChanged()
{
    return field<uint8_t>(this, 0xdc);
}
