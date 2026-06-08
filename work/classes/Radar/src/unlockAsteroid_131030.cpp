#include "class.h"

void Radar::unlockAsteroid()
{
    F<int>(this, 0x0c) = 0;
}
