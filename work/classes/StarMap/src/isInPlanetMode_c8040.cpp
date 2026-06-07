#include "class.h"

bool StarMap::isInPlanetMode()
{
    return field<int32_t>(this, 4) == 3;
}
