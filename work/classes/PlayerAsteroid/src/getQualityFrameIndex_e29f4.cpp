#include "class.h"

int PlayerAsteroid::getQualityFrameIndex()
{
    return 7 - F<int>(this, 0x150);
}
