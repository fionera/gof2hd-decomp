#include "class.h"

void Sprite::setFrame(int frame)
{
    if (frame < 0) {
        frame = -frame;
    }

    field<int32_t>(this, 0x38) = frame;
    if (frame >= field<int32_t>(this, 0x3c)) {
        frame = frame % field<int32_t>(this, 0x3c);
        field<int32_t>(this, 0x38) = frame;
    }

    int frameWidth = field<int32_t>(this, 0x18);
    int frameHeight = field<int32_t>(this, 0x1c);
    int columns = field<int32_t>(this, 0x30);
    int row = frame / columns;

    int column = frame - row * columns;
    int frameY = row * frameHeight;
    int frameX = column * frameWidth;

    field<int32_t>(this, 0x28) = frameX;
    field<int32_t>(this, 0x2c) = frameY;
}
