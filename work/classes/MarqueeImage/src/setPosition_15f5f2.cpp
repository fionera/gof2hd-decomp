#include "class.h"

void MarqueeImage::setPosition(int x, int y)
{
    field<int32_t>(this, 0xc) = x;
    field<int32_t>(this, 0x10) = y;
}
