#include "class.h"

Sprite::~Sprite()
{
    void *frames = field<void *>(this, 0x0);
    if (frames != 0) {
        operator delete(frames);
    }
    field<void *>(this, 0x0) = 0;
}
