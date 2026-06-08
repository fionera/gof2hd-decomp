#include "class.h"

void MarqueeImage::draw()
{
    return draw(field<int32_t>(this, 0xc), field<int32_t>(this, 0x10));
}
