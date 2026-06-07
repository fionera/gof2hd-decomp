#include "class.h"

void Sprite::defineReferencePixel(int x, int y)
{
    field<int32_t>(this, 0x8) = x;
    field<int32_t>(this, 0xc) = y;
}
