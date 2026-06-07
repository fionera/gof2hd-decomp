#include "class.h"

void Sprite::setRefPixelPosition(int x, int y)
{
    field<int32_t>(this, 0x10) = x;
    field<int32_t>(this, 0x14) = y;
}
