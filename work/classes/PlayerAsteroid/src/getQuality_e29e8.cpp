#include "class.h"

int PlayerAsteroid::getQuality()
{
    return F<int>(this, 0x150);
}
