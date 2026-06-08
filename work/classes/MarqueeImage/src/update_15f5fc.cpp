#include "class.h"

void MarqueeImage::update(int dt)
{
    float position = field<float>(this, 0x18);
    float delta = ((float)dt / 1000.0f) * field<float>(this, 0x1c);
    float width = (float)field<int32_t>(this, 4);
    position += delta;
    *(volatile float *)((char *)this + 0x18) = position;
    if (width < position) {
        position = (position - width) + delta * 0.5f;
        *(volatile float *)((char *)this + 0x18) = position;
    }

    field<int32_t>(this, 0x20) = (int)(width - position);
}
