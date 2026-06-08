#include "class.h"

extern "C" int GameText_getLanguage();

void NewsTicker::update(int dt)
{
    if (F<uint8_t>(this, 0x28) != 0) {
        return;
    }

    float step = ((float)dt / 1000.0f) * 50.0f;
    if (GameText_getLanguage() == 9) {
        float x = F<float>(this, 0x0) + step;
        float width = (float)F<int>(this, 0x0c);
        F<float>(this, 0x0) = x;
        if (x > width) {
            F<float>(this, 0x0) = (float)-F<int>(this, 0x10);
        }
    } else {
        float x = F<float>(this, 0x0) - step;
        float minX = (float)-F<int>(this, 0x10);
        F<float>(this, 0x0) = x;
        if (x < minX) {
            F<int>(this, 0x0) = 0;
        }
    }
}
