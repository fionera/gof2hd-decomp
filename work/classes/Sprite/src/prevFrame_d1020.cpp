#include "class.h"

void Sprite::prevFrame()
{
    return setFrame(field<int32_t>(this, 0x38) - 1);
}
