#include "class.h"

void Sprite::nextFrame()
{
    return setFrame(field<int32_t>(this, 0x38) + 1);
}
