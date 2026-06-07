#include "class.h"

void PlayerAsteroid::setAsteroidIndex(int asteroidIndex)
{
    F<int>(this, 0x128) = asteroidIndex;
}
