#include "class.h"

void ObjectGun::setScaling(int x, int y)
{
    F<float>(this, 0x3c) = (float)x;
    F<float>(this, 0x40) = (float)y;
}
